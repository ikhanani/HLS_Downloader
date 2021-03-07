#ifndef PERSISTER_H
#define PERSISTER_H
#include "HLSObjects.h"
using namespace boost::filesystem;

class Persister{
    
    public:
        virtual void Persist() = 0;

    private:
        path IODirectory;
        MediaFile MFile;
};
#endif