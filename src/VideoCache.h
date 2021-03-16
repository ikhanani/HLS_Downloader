#include "Poco/LRUCache.h"
#include <unordered_map>
#include "Poco/Logger.h"

using Poco::Logger;
using Poco::Message;
using namespace std;

class VideoCache{
    public:
        explicit VideoCache(int cs){
            cacheSize = cs;
        }
        bool checkFile(string url, string hash);

    private:
        unordered_map<string, std::unique_ptr<Poco::LRUCache<std::string, int>>> urlMap;
        int cacheSize;
};