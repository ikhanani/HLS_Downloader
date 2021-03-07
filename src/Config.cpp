#include "Config.h"
#include <fstream>
using namespace hls;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

void Config::read(string str)
{

    Parser p;
    Var result = p.parse(str);
    Object::Ptr object = result.extract<Object::Ptr>();
    setEnableLocal(findBool(object, "enableLocal"));
    setDir(findString(object, "baseDir"));
    vector<string> urlList = findList(object, "urls");
    for_each(urlList.begin(), urlList.end(), [&](string value) { this->urls.push_back(value); });
    level = findString(object, "level");
    format = findString(object, "format");
    rotation = findString(object, "rotation");
    archive = findString(object, "archive");
    cacheSize = std::stoi(findString(object, "cacheSize"));
    enableAWS = findBool(object, "enableAWS");
}

bool Config::findBool(Object::Ptr object, string key)
{

    Var var = object->get(key);
    return var;
}

string Config::findString(Object::Ptr object, string key)
{

    Var var = object->get(key);
    return var;
}

vector<string> Config::findList(Object::Ptr object, string key)
{

    vector<string> result;
    Poco::DynamicStruct ds = *object;
    Var var = ds[key];

    if (var.isArray())
    {
        for (int i = 0; i < var.size(); i++)
        {
            string val = var[i];
            result.push_back(val);
        }
    }

    return result;
}
