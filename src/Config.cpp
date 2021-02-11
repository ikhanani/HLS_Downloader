#include "Config.h"
#include "cpprest/json.h"
#include <fstream>
using namespace web;
using namespace utility;


void Config::read(string str){
    stringstream_t s(str);
    json::value v = json::value::parse(s);
    setEnableLocal(findBool(v, "enableLocal"));
    setDir(findString(v,"baseDir"));
    vector<string> urlList = findList(v, "urls");
    for_each(urlList.begin(), urlList.end(), [&] (string value){this->urls.push_back(value); });

}
bool Config::findBool(json::value v, string key){
    bool returnBool;
    try{
        json::value boolVal = v.at(U(key));
        returnBool = boolVal.as_bool();
    }
    catch(json::json_exception &e){
        cout << e.what();
        returnBool = false;
    }
    return returnBool;
}

string Config::findString(web::json::value v, string key){
    string returnStr;
    try{

        returnStr = v.at(U(key)).as_string();
    }
    catch(json::json_exception &e){
        cout << e.what();
    }
    return returnStr;
}
vector<string> Config::findList(web::json::value v, string key){
    json::array arr = v.at(U(key)).as_array();
    vector<string> result;
    //for(json::value i:arr){
        //getUrls().push_back(i.as_string());
    //}
    for(json::value i:arr){
        result.push_back(i.as_string());
    }
    return result;
}

