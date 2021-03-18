#include "Converter.h"
#include "Job.h"
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <streambuf>
#include "Poco/Logger.h"
#include "Poco/SplitterChannel.h"
#include "Poco/ConsoleChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include <cstdint>
#include <experimental/filesystem>

using namespace std;
using namespace boost::uuids;
using Poco::Logger;
using Poco::SplitterChannel;
using Poco::ConsoleChannel;
using Poco::FileChannel;
using Poco::AutoPtr;
using Poco::Message;
using Poco::PatternFormatter;
using Poco::FormattingChannel;

void initLoggers(const string &level, const string &format, const string &rotation, const string &archive) {
    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
    AutoPtr<FileChannel> pFile(new FileChannel("test.log"));
    pFile->setProperty("rotation", rotation);
    pFile->setProperty("archive", archive);
    AutoPtr<SplitterChannel> pSplitter(new SplitterChannel);
    AutoPtr<PatternFormatter> pPF(new PatternFormatter);
    pPF->setProperty("pattern", format);
    AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pCons));
    AutoPtr<FormattingChannel> pFF(new FormattingChannel(pPF, pFile));
    pSplitter->addChannel(pFC);
    pSplitter->addChannel(pFF);
    Logger::root().setChannel(pSplitter);
    Logger::root().setLevel(level);

}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " ConfigFile" << std::endl;
        return 1;
    }

    string configFile = argv[1];

    if (!boost::filesystem::exists(boost::filesystem::path(configFile))) {
        std::cerr << "ConfigFile" << configFile << " not found." << std::endl;
        return 1;
    }
    std::ifstream f(configFile);
    std::stringstream json;
    json << f.rdbuf();
    hls::Config configJson;
    configJson.read(json.str());
    VideoCache cache(configJson.getCacheSize());
    std::chrono::milliseconds duration(configJson.getDelay());
    while (true) {
        string path;
        string base;
        initLoggers(configJson.getLevel(), configJson.getFormat(), configJson.getRotation(), configJson.getArchive());
        Logger &root = Logger::root();
        root.debug("test");
        vector<string> urls = configJson.getUrls();
        string baseDir = configJson.getDir();
        int index;
        vector<std::thread> threadVector;
        Aws::SDKOptions options;
        Aws::InitAPI(options);

        for (auto &url : urls) {
            index = url.find_last_of('/');
            base = url.substr(0, index);
            path = url.substr(index + 1);
            Job j(base, path, baseDir, &cache, configJson);
            thread t(j);
            threadVector.push_back(std::move(t));
        }

        for (auto &t : threadVector) {
            if (t.joinable()) {
                t.join();
            }
        }

        Aws::ShutdownAPI(options);
        std::this_thread::sleep_for(duration);
    }
}