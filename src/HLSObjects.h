#ifndef HLSOBJECTS_H
#define HLSOBJECTS_H

#include <string>
#include <vector>
#include <memory>
using namespace std;

class MediaFile;
class ChunkList{
    public:
        int getVersion(){return version;}
        int getBandwidth(){return bandwidth;}
        string getCodecs(){return codecs;}
        string getResolution(){return resolution;}
        string getChunkName(){return chunkName;}
        void setVersion(int v){version = v;}
        void setBandwidth(int b){bandwidth = b;}
        void setCodecs(string c){codecs = c;}
        void setResolution(string r){resolution = r;}
        void setChunkName(string c){chunkName = c;}
    private:
        int version;
        int bandwidth;
        string codecs;
        string resolution;
        string chunkName;

};
class PlayList{
    public:
        int getVersion(){return version;}
        bool getAllowCache(){return allowCache;}
        int getTargetDuration(){return targetDuration;}
        int getMediaSequence(){return mediaSequence;}
        void setVersion(int v){version = v;}
        void setAllowCache(bool a){allowCache = a;}
        void setTargetDuration(int t){targetDuration = t;}
        void setMediaSequence(int m){mediaSequence = m;}
        vector<unique_ptr<MediaFile>> getFiles(){return std::move(files);}
        void add(unique_ptr<MediaFile> mediaF);
       
    private:
        vector<unique_ptr<MediaFile>> files;
        int version;
        bool allowCache;
        int targetDuration;
        int mediaSequence;


};
class MediaFile{
    public:
        double getDuration(){return duration;}
        string getName(){return name;}
        void setDuration(double d){duration = d;}
        void setName(string n){name = n;}
    private:
        double duration;
        string name;
};

#endif