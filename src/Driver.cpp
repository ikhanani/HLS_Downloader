#include "VideoFetcher.h"
#include "Converter.h"
#include <fstream>
using namespace std;
using namespace Concurrency::streams;

int main(){
    //ofstream file;
    //file.open("output.txt");
    string URL = "https://itsvideo.arlingtonva.us:8013/live/cam257.stream";
    string path = "playlist.m3u8";
    //ofstream* outfile = new std::ofstream("output.txt", ofstream::binary);
    //stdio_ostream<unsigned char>* output_stream = new stdio_ostream<unsigned char> (*outfile);
    concurrency::streams::container_buffer<std::vector<uint8_t>> buffer;
    concurrency::streams::ostream outfile(buffer);
    VideoFetcher fetcher = VideoFetcher(URL, path);
    fetcher.fetch(&outfile, URL + "/" + path);
    Converter c;
    ChunkList chunk = c.chunkConverter(buffer.collection());
    concurrency::streams::container_buffer<std::vector<uint8_t>> buffer2;
    concurrency::streams::ostream outfile2(buffer2);
    fetcher.fetch(&outfile2, URL + "/" + chunk.getChunkName());
    PlayList play = c.playListConverter(buffer2.collection());
    for(MediaFile i:play.getFiles()){
        ofstream* outfile = new std::ofstream(i.getName(), ofstream::binary);
        stdio_ostream<unsigned char>* output_stream = new stdio_ostream<unsigned char> (*outfile);
        fetcher.fetch(output_stream, URL + "/" + i.getName());
    }
    

}
// ostream output_file()