#include "Config.h"
#include <fstream>
using namespace hls;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

void Config::read(string str){
    /**stringstream_t s(str);
    json::value v = json::value::parse(s);
    setEnableLocal(findBool(v, "enableLocal"));
    setDir(findString(v,"baseDir"));
    vector<string> urlList = findList(v, "urls");
    for_each(urlList.begin(), urlList.end(), [&] (string value){this->urls.push_back(value); });
    **/
   Parser p;
   Var result = p.parse(str);
   Object::Ptr object = result.extract<Object::Ptr>();
   setEnableLocal(findBool(object, "enableLocal"));
    setDir(findString(object,"baseDir"));
    vector<string> urlList = findList(object, "urls");
    for_each(urlList.begin(), urlList.end(), [&] (string value){this->urls.push_back(value); });
}
bool Config::findBool(Object::Ptr object, string key){
    /**bool returnBool;
    try{
        json::value boolVal = v.at(U(key));
        returnBool = boolVal.as_bool();
    }
    catch(json::json_exception &e){
        cout << e.what();
        returnBool = false;
    }
    return returnBool;
    **/
   Var var = object->get(key);
   return var;

}

string Config::findString(Object::Ptr object, string key){
    /**string returnStr;
    try{

        returnStr = v.at(U(key)).as_string();
    }
    catch(json::json_exception &e){
        cout << e.what();
    }
    return returnStr;**/
    Var var = object->get(key);
   return var;
}
vector<string> Config::findList(Object::Ptr object, string key){
    /**json::array arr = v.at(U(key)).as_array();
    vector<string> result;
    for(json::value i:arr){
        result.push_back(i.as_string());
    }
    return result;**/
    vector<string> result;
    Poco::DynamicStruct ds = *object;
    Var var = ds[key];
    if(var.isArray()){
        for(int i = 0; i<var.size(); i++){
            string val = var[i];
            result.push_back(val);
        }
    }
    return result;
}

