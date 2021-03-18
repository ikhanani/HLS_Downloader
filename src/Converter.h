#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include "HLSObjects.h"
#include <vector>
#include <memory>

using namespace std;

class Converter {

public:
    unique_ptr<ChunkList> chunkConverter(const string &str);

    unique_ptr<PlayList> playListConverter(const string &str);

private:
    const string ver = "#EXT-X-VERSION:";
    const string band = "BANDWIDTH";
    const string codec = "CODECS";
    const string res = "RESOLUTION";
    const string cache = "#EXT-X-ALLOW-CACHE:";
    const string target = "#EXT-X-TARGETDURATION:";
    const string media = "#EXT-X-MEDIA-SEQUENCE:";
    const string dur = "#EXTINF:";

};

#endif