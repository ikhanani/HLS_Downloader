// Server code from https://github.com/pocoproject/poco/tree/3fc3e5f5b8462f7666952b43381383a79b8b5d92/Net/samples/HTTPTimeServer

//
// HTTPTimeServer.cpp
//
// This sample demonstrates the HTTPServer and related classes.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <utility>
#include <condition_variable>

using Poco::Net::ServerSocket;
using Poco::Net::SocketAddress;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::HTTPServerParams;
using Poco::Timestamp;
using Poco::DateTimeFormatter;
using Poco::DateTimeFormat;
using Poco::ThreadPool;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


class TimeRequestHandler: public HTTPRequestHandler
	/// Return a HTML document with the current date and time.
{
public:
	explicit TimeRequestHandler(std::string  format):
		_format(std::move(format))
	{
	}
	
	void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override
	{
		Application& app = Application::instance();
		app.logger().information("Request from " + request.clientAddress().toString());

		Timestamp now;
		std::string dt(DateTimeFormatter::format(now, _format));

		response.setChunkedTransferEncoding(true);
		response.setContentType("text/html");
		std::cout << request.getURI() << "\n";
		std::ostream& ostr = response.send();
		if (request.getURI() == "/step1"){
			ostr << "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-STREAM-INF:BANDWIDTH=440283,CODECS= avc1.77.20 ,RESOLUTION=352x240\nstep2";
		}
		if (request.getURI() == "/step2"){
			ostr << "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-ALLOW-CACHE:NO\n#EXT-X-TARGETDURATION:11\n#EXT-X-MEDIA-SEQUENCE:1010\n#EXTINF:10.01,\nfile1\n#EXTINF:10.01,\nfile2";
		}
		if (request.getURI() == "/file1"){
			ostr << "This is file 1";
		}
		if (request.getURI() == "/file2"){
			ostr << "This is file 2";
		}
	}
	
private:
	std::string _format;
};


class HLSRequestHandlerFactory: public HTTPRequestHandlerFactory
{
public:
	HLSRequestHandlerFactory(const std::string& format):
		_format(format)
	{
	}

	HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
	{
		return new TimeRequestHandler(_format);
	}
	
private:
	std::string _format;
};


class HLSTestServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the HTTPTimeServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (HTTPTimeServer.properties),
	/// copy the file to the directory where the HTTPTimeServer executable
	/// resides. If you start the debug version of the HTTPTimeServer
	/// (HTTPTimeServerd[.exe]), you must also create a copy of the configuration
	/// file named HTTPTimeServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9980) and the format of the date/time string sent back to the client.
	///
	/// To test the TimeServer you can use any web browser (http://localhost:9980/).
{
public:
	HLSTestServer(std::mutex *mu, std::condition_variable *cva, bool *r): _helpRequested(false)
	{
		m = mu;
		cv = cva;
		ready = r;
	}
	
	~HLSTestServer()
	{
	}

protected:
	void initialize(Application& self)
	{
		std::cout << "Initialize" << "\n";
		std::lock_guard<std::mutex> lg(*m);
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
		
	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);
		
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A web server that serves the current date and time.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		std::cout << "Server start" << "\n";
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			std::cout << "main" << "\n";
			// get parameters from configuration file
			unsigned short port = (unsigned short) config().getInt("HTTPTimeServer.port", 9980);
			std::string format(config().getString("HTTPTimeServer.format", DateTimeFormat::SORTABLE_FORMAT));
			int maxQueued  = config().getInt("HTTPTimeServer.maxQueued", 100);
			int maxThreads = config().getInt("HTTPTimeServer.maxThreads", 16);
			ThreadPool::defaultPool().addCapacity(maxThreads);
			
			HTTPServerParams* pParams = new HTTPServerParams;
			pParams->setMaxQueued(maxQueued);
			pParams->setMaxThreads(maxThreads);
			
			SocketAddress sa(SocketAddress::IPv4, "0.0.0.0", 9980);
			// set-up a server socket
			ServerSocket svs(sa);
			// set-up a HTTPServer instance
			HTTPServer srv(new HLSRequestHandlerFactory(format), svs, pParams);
			*ready = true;
			cv->notify_all();
			// start the HTTPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the HTTPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}
	
private:
	bool _helpRequested;
	std::mutex *m;
	std::condition_variable *cv;
	bool *ready;
};


/**int main(int argc, char** argv)
{
	HLSTestServer app;
	return app.run(argc, argv);
}
**/