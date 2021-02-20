#include "Job.h"
#include <fstream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <streambuf>
using namespace std;
using namespace Concurrency::streams;
using namespace boost::uuids;
void Job::operator()(){
    concurrency::streams::container_buffer<std::vector<uint8_t>> buffer;
    concurrency::streams::ostream outfile(buffer);
    VideoFetcher fetcher = VideoFetcher(base, path);
    fetcher.fetch(&outfile, base + "/" + path);
    Converter c;
    unique_ptr<ChunkList> chunk = c.chunkConverter(buffer.collection());
    concurrency::streams::container_buffer<std::vector<uint8_t>> buffer2;
    concurrency::streams::ostream outfile2(buffer2);
    fetcher.fetch(&outfile2, base + "/" + chunk->getChunkName());
    unique_ptr<PlayList> play = c.playListConverter(buffer2.collection());
    S3Persister s("hls-dataset", "us-east-1");
    uuid id; 
    std::stringstream idStr;
    for(int i = 0; i < play->getFiles().size(); i++){
        vector<shared_ptr<MediaFile>> files = play->getFiles();
        ofstream* outfile = new std::ofstream(files.at(i)->getName(), ofstream::binary);
        stdio_ostream<unsigned char>* output_stream = new stdio_ostream<unsigned char> (*outfile);
        fetcher.fetch(output_stream, base + "/" + play->getFiles()[i]->getName());
        id = random_generator()();
        idStr << id;
        s.upload(idStr.str(), play->getFiles()[i]->getName());
        idStr.str("");
        }
}