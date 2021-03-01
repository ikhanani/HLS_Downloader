#include "FilesystemPersister.h"
void FilesystemPersister::Persist(){
    std::ofstream* outfile = new std::ofstream(IODirectory.string() + "/" + MFile->getName(), std::ofstream::binary);
    output_stream = outfile;
}
std::ostream* FilesystemPersister::getOStream(){
    return output_stream;
}