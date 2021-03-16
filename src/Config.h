#include <string>
#include <utility>
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
        void read(const string& str);
        bool getEnableLocal() const { return enableLocal; }
        string getDir() { return dir; }
        vector<string> getUrls() { return urls; }
        string getLevel(){return level;}
        string getFormat(){return format;}
        string getRotation(){return rotation;}
        string getArchive(){return archive;}
        int getCacheSize() const{return cacheSize;}
        bool getEnableAWS() const { return enableAWS;}
        string getBucketName(){return bucketName;}
        string getAWSRegion(){return AWSRegion;}
        int getDelay(){return delay;}
        void setEnableLocal(bool e) { enableLocal = e; }
        void setDir(string d) { dir = d; }
        static bool findBool(Poco::JSON::Object::Ptr object, const string& key);
        static string findString(Poco::JSON::Object::Ptr object, const string& key);
        static vector<string> findList(Poco::JSON::Object::Ptr object, const string& key);

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
        string bucketName;
        string AWSRegion;
        int delay;
    };
} // namespace hls