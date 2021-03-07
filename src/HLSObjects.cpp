#include "HLSObjects.h"
using namespace boost::filesystem;

void PlayList::add(std::shared_ptr<MediaFile> mediaF){
    files.push_back(mediaF);
}

path MediaFile::getPath(string s){
    string value;
    s = s.substr(s.find("://") + 3);
    replace(s.begin(), s.end(), '/', ' ');
    path p = {""};
    istringstream linestream(s);

    while (linestream >> value){
        p.append(value);
    }

    return p;
       
}