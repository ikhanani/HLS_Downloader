#include "Converter.h"
#include "Config.h"
#include "Job.h"
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <streambuf>
using namespace std;
using namespace boost::uuids;

int main(){
    string path;
    string base; 
    std::ifstream f("../config/config.json");
    std::stringstream json;
    json << f.rdbuf();
    hls::Config configJson;
    configJson.read(json.str());
    vector<string> urls = configJson.getUrls();
    string baseDir = configJson.getDir();
    int index;
    vector<std::thread> threadVector;
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    for(int i = 0; i < urls.size(); i++){
        index = urls.at(i).find_last_of("/");
        base = urls.at(i).substr(0, index);
        path = urls.at(i).substr(index + 1);
        Job j(base, path, baseDir);
        thread t(j);
        threadVector.push_back(std::move(t));
    }

    for(auto& t : threadVector){
        if (t.joinable()){
            t.join();
        }
    }
    Aws::ShutdownAPI(options);
}
