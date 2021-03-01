#include "../src/S3Persister.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <gtest/gtest.h>
using namespace boost::uuids;

/**TEST(S3Test, putObjectTest){
    S3Persister s("hls-dataset", "us-east-1");
    uuid id = random_generator()();
    stringstream idStr;
    idStr << id;
    uuid filename = random_generator()();
    stringstream fileStr;
    fileStr << filename;
    std::ofstream out(fileStr.str());
    out << "This is a test";
    out.close();
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    s.upload(idStr.str(), fileStr.str());
    Aws::ShutdownAPI(options);

}**/