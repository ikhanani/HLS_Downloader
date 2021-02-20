#include "HLSObjects.h"

void PlayList::add(std::shared_ptr<MediaFile> mediaF){
    files.push_back(mediaF);
}