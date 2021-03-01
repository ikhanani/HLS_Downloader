#include "VideoFetcher.h"
#include <iostream>

VideoFetcher::VideoFetcher(string baseUrl, string playListPath){
    this->baseUrl = baseUrl;
    this->playListPath = playListPath;

}
unique_ptr<PlayList> VideoFetcher::fetchPlayList(){
    ostringstream out;
    VideoFetcher fetcher = VideoFetcher(baseUrl, playListPath);
    fetcher.fetch(out, baseUrl + "/" + playListPath);
    Converter c;
    unique_ptr<ChunkList> chunk = c.chunkConverter(out.str());
    ostringstream out2;
    fetcher.fetch(out2, baseUrl + "/" + chunk->getChunkName());
    unique_ptr<PlayList> play = c.playListConverter(out2.str());
    return play;
}

void VideoFetcher::fetch(std::ostream& outputStream, string targetUrl)
{
   /** http_client client(targetUrl);
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
    }**/

    URI uri(targetUrl);
    HTTPClientSession* session;

    if(targetUrl.find("https")!=-1){
        session = new HTTPSClientSession(uri.getHost(), uri.getPort());
        //HTTPSClientSession session(uri.getHost(), uri.getPort());
    }
    else{
        session = new HTTPClientSession(uri.getHost(), uri.getPort());
    }
    HTTPRequest request(HTTPRequest::HTTP_GET, targetUrl, HTTPMessage::HTTP_1_1);
    HTTPResponse response;
    session->sendRequest(request);
	std::istream& rs = session->receiveResponse(response);
	std::cout << response.getStatus() << " " << response.getReason() << std::endl;
    StreamCopier::copyStream(rs, outputStream);
    delete session;
}

