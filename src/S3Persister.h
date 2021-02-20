#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h> 


using namespace Aws;
using namespace Aws::S3;
using namespace Aws::S3::Model;
class S3Persister{
    public:
        S3Persister(string n, string r);
        void upload(string key, string path);

    private:
        string name;
        string region;
        string bucketName;

};