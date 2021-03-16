#include "Job.h"
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <streambuf>
#include "Config.h"
#include <ctime>
using namespace std;

void Job::operator()(){
    time_t theTime = time(0);
    std::ifstream f("../config/config.json");
    std::stringstream json;
    json << f.rdbuf();
    hls::Config configJson;
    configJson.read(json.str());
    VideoFetcher fetcher = VideoFetcher(base, path);
    unique_ptr<PlayList> play = fetcher.fetchPlayList();
    string currentHash;

    for(int i = 0; i < play->getFiles().size(); i++){
        vector<shared_ptr<MediaFile>> files = play->getFiles();
        boost::filesystem::path p = files[i]->getPath(base, theTime);
        FilesystemPersister f(files[i], files[i]->getPath(base, theTime));
        f.Persist();
        currentHash = fetcher.fetch(*f.getOStream(), base + "/" + play->getFiles()[i]->getName());
        if(cache->checkFile(base, currentHash)){
            boost::filesystem::remove(f.getFullPath());

        }
        else{
            if(configJson.getEnableAWS()){
                S3Persister s(configJson.getBucketName(), configJson.getAWSRegion(), files[i]->getPath(base, theTime).string() + "/" + play->getFiles()[i]->getName(), files[i]->getPath(base, theTime).string() + "/" + play->getFiles()[i]->getName());
                s.Persist();
            }
            if(!configJson.getEnableLocal()){
                boost::filesystem::remove(f.getFullPath());
            }
        }
        }
}