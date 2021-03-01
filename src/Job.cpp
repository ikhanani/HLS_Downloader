#include "Job.h"
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <streambuf>
using namespace std;
void Job::operator()(){
    /**concurrency::streams::container_buffer<std::vector<uint8_t>> buffer;
    concurrency::streams::ostream outfile(buffer);
    VideoFetcher fetcher = VideoFetcher(base, path);
    fetcher.fetch(&outfile, base + "/" + path);
    Converter c;
    unique_ptr<ChunkList> chunk = c.chunkConverter(buffer.collection());
    concurrency::streams::container_buffer<std::vector<uint8_t>> buffer2;
    concurrency::streams::ostream outfile2(buffer2);
    fetcher.fetch(&outfile2, base + "/" + chunk->getChunkName());
    unique_ptr<PlayList> play = c.playListConverter(buffer2.collection());
    uuid id; 
    std::stringstream idStr;**/
    VideoFetcher fetcher = VideoFetcher(base, path);
    unique_ptr<PlayList> play = fetcher.fetchPlayList();
    for(int i = 0; i < play->getFiles().size(); i++){
        vector<shared_ptr<MediaFile>> files = play->getFiles();
        FilesystemPersister f(files[i], files[i]->getPath(base));
        f.Persist();
        fetcher.fetch(*f.getOStream(), base + "/" + play->getFiles()[i]->getName());
        S3Persister s("hls-dataset", "us-east-1",files[i]->getPath(base).string() + "/" + play->getFiles()[i]->getName(), files[i]->getPath(base).string() + "/" + play->getFiles()[i]->getName());
        s.Persist();
        }
}