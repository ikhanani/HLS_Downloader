#include "Persister.h"
#include <fstream>
#include <streambuf>
#include <iostream>
#include <ctime>

class FilesystemPersister : public Persister {
public:
    FilesystemPersister(std::shared_ptr<MediaFile> f, path dir);

    std::shared_ptr<std::ostream> getOStream();

    void Persist() override;

    string getFullPath();

    void Delete(string s) override;

private:
    std::shared_ptr<std::ostream> output_stream;
    path IODirectory;
    std::shared_ptr<MediaFile> MFile;
};