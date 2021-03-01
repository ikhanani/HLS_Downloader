#include "VideoFetcher.h"
#include "Converter.h"
#include "FilesystemPersister.h"
#include "S3Persister.h"
class Job{
    public:
        Job(string b, string p, string bd){
            base = b;
            path = p;
            baseDir = bd;
        }
        void operator()();
    private:
        string base;
        string path;
        string baseDir;
};