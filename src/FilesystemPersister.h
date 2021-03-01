#include "Persister.h"
#include <fstream>
#include <streambuf>
#include <iostream>
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
        std::ostream* getOStream();
        void Persist();
    private:
        std::ostream* output_stream;
        path IODirectory;
        std::shared_ptr<MediaFile> MFile;
};