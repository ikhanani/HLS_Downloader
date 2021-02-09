#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/interopstream.h>
using namespace std;
class VideoFetcher{
    public:
    VideoFetcher(string baseUrl, string playListPath);
    void fetch(concurrency::streams::ostream* outputStream, string targetUrl);
    private:
    string baseUrl;
    string playListPath;
};