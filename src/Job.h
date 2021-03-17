#include <utility>

#include "VideoFetcher.h"
#include "Converter.h"
#include "FilesystemPersister.h"
#include "S3Persister.h"
#include "VideoCache.h"

class Job {

public:
    Job(string b, string p, string bd, VideoCache *c) {
        base = b;
        path = p;
        baseDir = bd;
        cache = c;
    }

    void operator()();

private:
    string base;
    string path;
    string baseDir;
    VideoCache *cache;
};