#include <string>
#include <iostream>
#include <fstream>
#include "Persister.h"
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include "Poco/Logger.h"

using Poco::Logger;
using Poco::Message;
using namespace std;
using namespace Aws;
using namespace Aws::S3;
using namespace Aws::S3::Model;

class S3Persister : public Persister {

public:
    S3Persister(string n, string r, string k, string p);

    void upload(string key, string path);

    void Persist() override;

    void Delete(string s) override;

private:
    string name;
    string region;
    string bucketName;
    string key;
    string path;

};