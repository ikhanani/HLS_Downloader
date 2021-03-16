#ifndef HLSOBJECTS_H
#define HLSOBJECTS_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <sstream>
#include <boost/filesystem.hpp>
#include <ctime>
using namespace std;

class MediaFile;

class ChunkList{

    public:
        int getVersion() const{return version;}
        int getBandwidth() const{return bandwidth;}
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
        int getVersion() const{return version;}
        bool getAllowCache() const{return allowCache;}
        int getTargetDuration() const{return targetDuration;}
        int getMediaSequence() const{return mediaSequence;}
        void setVersion(int v){version = v;}
        void setAllowCache(bool a){allowCache = a;}
        void setTargetDuration(int t){targetDuration = t;}
        void setMediaSequence(int m){mediaSequence = m;}
        vector<shared_ptr<MediaFile>> getFiles(){return files;}
        void add(const shared_ptr<MediaFile>& mediaF);
       
    private:
        vector<shared_ptr<MediaFile>> files;
        int version;
        bool allowCache;
        int targetDuration;
        int mediaSequence;


};
class MediaFile{
    
    public:
        double getDuration() const{return duration;}
        string getName(){return name;}
        string getFullUrl(){return fullUrl;}
        void setDuration(double d){duration = d;}
        void setName(string n){name = n;}
        void setFullUrl(string u){fullUrl = u;}
        static boost::filesystem::path getPath(string s, time_t t);

    private:
        double duration;
        string name;
        string fullUrl;
};

#endif