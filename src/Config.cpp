#include "Config.h"
#include <fstream>

using namespace hls;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

void Config::read(const string &str) {

    Parser p;
    Var result = p.parse(str);
    const auto &object = result.extract<Object::Ptr>();
    setEnableLocal(findBool(object, "enableLocal"));
    setDir(findString(object, "baseDir"));
    vector<string> urlList = findList(object, "urls");
    for_each(urlList.begin(), urlList.end(), [&](const string &value) { this->urls.push_back(value); });
    level = findString(object, "level");
    format = findString(object, "format");
    rotation = findString(object, "rotation");
    archive = findString(object, "archive");
    cacheSize = std::stoi(findString(object, "cacheSize"));
    enableAWS = findBool(object, "enableAws");
    bucketName = findString(object, "bucketName");
    AWSRegion = findString(object, "AwsRegion");
    delay = std::stoi(findString(object, "delay"));
}

bool Config::findBool(Object::Ptr object, const string &key) {

    Var var = object->get(key);
    return var;
}

string Config::findString(Object::Ptr object, const string &key) {

    Var var = object->get(key);
    return var;
}

vector<string> Config::findList(Object::Ptr object, const string &key) {

    vector<string> result;
    Poco::DynamicStruct ds = *object;
    Var var = ds[key];

    if (var.isArray()) {
        for (const auto &i : var) {
            string val = i;
            result.push_back(val);
        }
    }

    return result;
}
