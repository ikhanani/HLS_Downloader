#include "S3Persister.h"
#include <sys/stat.h>
using namespace Aws;
using namespace Aws::S3;
using namespace Aws::S3::Model;
S3Persister::S3Persister(string n, string r){
    this->name = n;
    this->region = r;

}
void S3Persister::upload(string key, string path){
    struct stat buffer;
     if (stat(path.c_str(), &buffer) == -1)
    {
        std::cout << "Error: PutObject: File '" <<
            path << "' does not exist." << std::endl;

        //return false;
    }

    Client::ClientConfiguration config;
    config.region = this->region;
    Aws::S3::S3Client s3_client(config);
    Aws::S3::Model::PutObjectRequest request;
    Aws::String bucket(this->name.c_str(), this->name.size());
    Aws::String awsPath(path.c_str(), path.size());
    request.SetBucket(bucket);
    request.SetKey(awsPath);
    std::shared_ptr<Aws::IOStream> input_data = 
        Aws::MakeShared<Aws::FStream>("SampleAllocationTag", 
            awsPath.c_str(), 
            std::ios_base::in | std::ios_base::binary);

    request.SetBody(input_data);
    Aws::S3::Model::PutObjectOutcome outcome = s3_client.PutObject(request);
    if (outcome.IsSuccess()) {

        std::cout << "Added object '" << key << "' to bucket '"
            << this->name << "'.";
        //return true;
    }
    else 
    {
        std::cout << "Error: PutObject: " << 
            outcome.GetError().GetMessage() << std::endl;
       
        //return false;
    }
    
}