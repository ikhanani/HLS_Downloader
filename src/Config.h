#include <string>
#include <vector>
#include "cpprest/json.h"
using namespace std;
class Config{
    public:
        void read(string str);
        bool getEnableLocal(){return enableLocal;}
        string getDir(){return dir;}
        vector<string> getUrls(){return urls;}
        void setEnableLocal(bool e){enableLocal = e;}
        void setDir(string d){dir = d;}
        bool findBool(web::json::value v, string key);
        string findString(web::json::value v, string key);
        vector<string> findList(web::json::value v, string key);
    
    private:
        bool enableLocal;
        string dir;
        vector<string> urls;


};