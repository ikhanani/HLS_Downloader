#include "Converter.h"
#include "Config.h"
#include "Job.h"
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <streambuf>
using namespace std;
using namespace Concurrency::streams;
using namespace boost::uuids;

int main(){
    //ofstream file;
    //file.open("output.txt");
    //string URL = "https://itsvideo.arlingtonva.us:8013/live/cam257.stream";
    string path;
    string base; 
    //ofstream* outfile = new std::ofstream("output.txt", ofstream::binary);
    //stdio_ostream<unsigned char>* output_stream = new stdio_ostream<unsigned char> (*outfile);
    ifstream f("../config/config.json");
    std::stringstream json;
    json << f.rdbuf();
    hls::Config configJson;
    configJson.read(json.str());
    vector<string> urls = configJson.getUrls();
    int index;
    vector<std::thread> threadVector;
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    for(int i = 0; i < urls.size(); i++){
        index = urls.at(i).find_last_of("/");
        base = urls.at(i).substr(0, index);
        path = urls.at(i).substr(index + 1);
        Job j(base, path);
        thread t(j);
        threadVector.push_back(std::move(t));
    }

    for(auto& t : threadVector){
        if (t.joinable()){
            t.join();
        }
    }
    Aws::ShutdownAPI(options);
    // for(int i = 0; i < threads.size(); i++){
    //     threads.at(i).join();
    // }


    //  for(int i = 0; i < threads.size(); i++){
    //     std::thread t = threads.at(i);
    //     if (t.joinable()){
    //         t.join();
    //     }
    // }
    

}
// ostream output_file()