#include "FilesystemPersister.h"
using namespace Concurrency::streams;
void FilesystemPersister::Persist(){
    std::ofstream* outfile = new std::ofstream(IODirectory.string() + "/" + MFile->getName(), std::ofstream::binary);
    output_stream = new stdio_ostream<unsigned char> (*outfile);
}