#include "Converter.h"
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

unique_ptr<ChunkList> Converter::chunkConverter(const string &str) {
    unique_ptr<ChunkList> chunk = make_unique<ChunkList>();
    istringstream stream(str);
    string line;
    string linecopy;
    string key;
    string value;

    while (getline(stream, line)) {

        if (line.find(ver) != -1) {
            chunk->setVersion(stoi(line.substr(line.find(ver) + 15)));
        }

        if (line.find("#EXT-X-STREAM-INF") != -1) {
            linecopy = line;
            replace(linecopy.begin(), linecopy.end(), ':', ' ');
            replace(linecopy.begin(), linecopy.end(), ',', ' ');
            replace(linecopy.begin(), linecopy.end(), '=', ' ');
            linecopy = linecopy.substr(18);
            istringstream linestream(linecopy);

            while (linestream >> key >> value) {

                if (key == band) {
                    chunk->setBandwidth(stoi(value));
                }

                if (key == codec) {
                    chunk->setCodecs(value.substr(0, value.length()));
                }

                if (key == res) {
                    chunk->setResolution(value);
                }
            }
        }

        if (line.find("chunklist") != -1 || line.find("step") != -1) {
            chunk->setChunkName(line);
        }
    }

    return chunk;


}

unique_ptr<PlayList> Converter::playListConverter(const string &str) {

    unique_ptr<PlayList> play = make_unique<PlayList>();
    istringstream stream(str);
    string line;
    double tempDur;
    bool readingFile = false;

    while (getline(stream, line)) {

        if (line.find(ver) != -1) {
            play->setVersion(stoi(line.substr(line.find(ver) + 15)));
        }

        if (line.find(cache) != -1) {
            play->setAllowCache(line.find("NO") == -1);
        }

        if (line.find(target) != -1) {
            play->setTargetDuration(stoi(line.substr(line.find(target) + 22)));
        }

        if (line.find(media) != -1) {
            play->setMediaSequence(stoi(line.substr(line.find(media) + 22)));
        }

        if (readingFile) {
            shared_ptr<MediaFile> temp = make_shared<MediaFile>();
            temp->setDuration(tempDur);
            temp->setName(line);
            readingFile = false;
            play->add(temp);
        }

        if (line.find(dur) != -1) {
            tempDur = stod(line.substr(8, line.length()));
            readingFile = true;
        }

    }

    return play;

}