#include "S3Persister.h"
#include <sys/stat.h>

using namespace Aws;
using namespace Aws::S3;
using namespace Aws::S3::Model;

S3Persister::S3Persister(string n, string r, string k, string p) {
    this->name = n;
    this->region = r;
    this->key = k;
    this->path = p;
}

void S3Persister::Persist() {
    Logger &root = Logger::root();
    struct stat buffer;

    if (stat(path.c_str(), &buffer) == -1) {
        root.information("Error: PutObject: File '" +
                         path + "' does not exist." + "\n");

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

        root.information("Added object '" + key + "' to bucket '"
                         + this->name + "'.");
    } else {
        root.information("Error: PutObject: " +
                         string(outcome.GetError().GetMessage()) + "\n");

    }

}