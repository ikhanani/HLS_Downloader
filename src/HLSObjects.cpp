#include "HLSObjects.h"

using namespace boost::filesystem;

void PlayList::add(const std::shared_ptr<MediaFile> &mediaF) {
    files.push_back(mediaF);
}

path MediaFile::getPath(string s, time_t t) {
    string value;
    struct tm *aTime = localtime(&t);
    s = s + "/" + std::to_string(aTime->tm_year + 1900) + "/" + std::to_string(aTime->tm_mon + 1) + "/" +
        std::to_string(aTime->tm_mday);
    s = s.substr(s.find("://") + 3);
    replace(s.begin(), s.end(), '/', ' ');
    path p = {""};
    istringstream linestream(s);

    while (linestream >> value) {
        p.append(value);
    }

    return p;

}