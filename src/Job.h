#include "VideoFetcher.h"
#include "Converter.h"
#include "S3Persister.h"
class Job{
    public:
        Job(string b, string p){
            base = b;
            path = p;
        }
        void operator()();
    private:
        string base;
        string path;
};