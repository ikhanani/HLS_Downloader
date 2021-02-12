#include "HLSObjects.h"

void PlayList::add(std::unique_ptr<MediaFile> mediaF){
    files.push_back(std::move(mediaF));
}