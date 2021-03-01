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
#include <iomanip>
using namespace std;
using namespace Poco::JSON;
namespace hls{
class Config{
    public:
        void read(string str);
        bool getEnableLocal(){return enableLocal;}
        string getDir(){return dir;}
        vector<string> getUrls(){return urls;}
        void setEnableLocal(bool e){enableLocal = e;}
        void setDir(string d){dir = d;}
        bool findBool(Object::Ptr object, string key);
        string findString(Object::Ptr object, string key);
        vector<string> findList(Object::Ptr object, string key);
    
    private:
        bool enableLocal;
        string dir;
        vector<string> urls;


};
}