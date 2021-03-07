#include <string>
#include "HLSObjects.h"
#include "Converter.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/DigestStream.h"
#include "Poco/MD5Engine.h"
#include "Poco/TeeStream.h"
#include "Poco/Logger.h"

using Poco::Logger;
using Poco::Message;
using Poco::DigestOutputStream;
using Poco::DigestEngine;
using Poco::MD5Engine;
using Poco::TeeOutputStream;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using Poco::Net::HTTPMessage;
using Poco::StreamCopier;
using Poco::Path;
using Poco::URI;
using Poco::Exception;
using namespace std;

class VideoFetcher{
    public:
        VideoFetcher(string baseUrl, string playListPath);
        string fetch(std::ostream &outputStream, string targetUrl);
        unique_ptr<PlayList> fetchPlayList();

    private:
        string baseUrl;
        string playListPath;
};