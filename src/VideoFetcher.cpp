#include "VideoFetcher.h"
using namespace web::http::client;
using namespace web::http;
using namespace web;
#include <iostream>

VideoFetcher::VideoFetcher(string baseUrl, string playListPath){
    this->baseUrl = baseUrl;
    this->playListPath = playListPath;

}

void VideoFetcher::fetch(concurrency::streams::ostream *outputStream, string targetUrl)
{
    //http_client client(U("https://itsvideo.arlingtonva.us:8013/live/cam257.stream"));
    http_client client(targetUrl);
    auto callBack = [=](http_response response) {
        std::cout << "Received response status code: " << response.status_code() << "\n";
        if (response.status_code() != 200)
        {
            throw runtime_error("Returned" + response.status_code());
        }
        http_headers headers = response.headers();
        auto bodyStream = response.body();
        return bodyStream.read_to_end(outputStream->streambuf());
    };

    pplx::task<size_t> response = client.request(methods::GET).then(callBack);
    try
    {
        response.wait();
    }
    catch (web::http::http_exception &e)
    {
        cout << "HTTP Exception: " << e.what() << "Error code: " << e.error_code() << "\n";
    }
}

