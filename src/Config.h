#include <string>
#include <vector>
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"
#include "Poco/Environment.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/StreamCopier.h"
#include "Poco/Stopwatch.h"
#include "Poco/Logger.h"
#include <iomanip>
using namespace std;
using namespace Poco::JSON;

namespace hls
{

    class Config
    {

    public:
        void read(string str);
        bool getEnableLocal() { return enableLocal; }
        string getDir() { return dir; }
        vector<string> getUrls() { return urls; }
        string getLevel(){return level;}
        string getFormat(){return format;}
        string getRotation(){return rotation;}
        string getArchive(){return archive;}
        int getCacheSize(){return cacheSize;}
        bool getEnableAWS() { return enableAWS;}
        void setEnableLocal(bool e) { enableLocal = e; }
        void setDir(string d) { dir = d; }
        bool findBool(Poco::JSON::Object::Ptr object, string key);
        string findString(Poco::JSON::Object::Ptr object, string key);
        vector<string> findList(Poco::JSON::Object::Ptr object, string key);

    private:
        bool enableLocal;
        string dir;
        vector<string> urls;
        string level;
        string format;
        string rotation;
        string archive;
        int cacheSize;
        bool enableAWS;
    };
} // namespace hls