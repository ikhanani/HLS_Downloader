#include "FilesystemPersister.h"

#include <utility>

FilesystemPersister::FilesystemPersister(std::shared_ptr<MediaFile> f, path dir) {
    IODirectory = dir;
    MFile = std::move(f);

    if (boost::filesystem::exists(IODirectory)) {

    } else {
        boost::filesystem::create_directories(IODirectory);
    }
}

void FilesystemPersister::Persist() {
    output_stream = make_shared<std::ofstream>(IODirectory.string() + "/" + MFile->getName(), std::ofstream::binary);
}

std::shared_ptr<std::ostream> FilesystemPersister::getOStream() {
    return output_stream;
}

string FilesystemPersister::getFullPath() {
    return IODirectory.string() + "/" + MFile->getName();
}

void FilesystemPersister::Delete(string s){
    boost::filesystem::remove(s);
}