#include "Persister.h"
#include <fstream>
#include <streambuf>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/interopstream.h>
class FilesystemPersister : public Persister{
    public:
        FilesystemPersister(std::shared_ptr<MediaFile> f, path dir){
            IODirectory = dir;
            MFile = f;
            if(boost::filesystem::exists(dir)){

            }
            else{
                boost::filesystem::create_directories(dir);
        }
        }
        void Persist();
        Concurrency::streams::stdio_ostream<unsigned char>* getOStream(){return output_stream;}
    private:
        Concurrency::streams::stdio_ostream<unsigned char>* output_stream;
        path IODirectory;
        std::shared_ptr<MediaFile> MFile;
};