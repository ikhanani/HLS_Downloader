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
    auto *outfile = new std::ofstream(IODirectory.string() + "/" + MFile->getName(), std::ofstream::binary);
    output_stream = outfile;
}

std::ostream *FilesystemPersister::getOStream() {
    return output_stream;
}

string FilesystemPersister::getFullPath() {
    return IODirectory.string() + "/" + MFile->getName();
}