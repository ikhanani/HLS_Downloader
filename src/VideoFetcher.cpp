#include "VideoFetcher.h"
#include <iostream>

VideoFetcher::VideoFetcher(string baseUrl, string playListPath) {
    this->baseUrl = baseUrl;
    this->playListPath = playListPath;
}

unique_ptr<PlayList> VideoFetcher::fetchPlayList() {

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

string VideoFetcher::fetch(std::ostream &outputStream, string targetUrl) {

    
    Logger &root = Logger::root();
    MD5Engine md5;
    DigestOutputStream ostr(md5);
    TeeOutputStream a(ostr);
    URI uri(targetUrl);
    std::string path(uri.getPathAndQuery());
    HTTPClientSession *session;
    a.addStream(outputStream);

    try{
        if (targetUrl.find("https") != -1) {
            session = new HTTPSClientSession(uri.getHost(), uri.getPort());
            //HTTPSClientSession session(uri.getHost(), uri.getPort());
        } else {
            session = new HTTPClientSession(uri.getHost(), uri.getPort());
        }

        HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
        HTTPResponse response;
        session->sendRequest(request);
        std::istream &rs = session->receiveResponse(response);
        stringstream out;
        out << response.getStatus() << " " << response.getReason() << std::endl;
        root.information(out.str());
        StreamCopier::copyStream(rs, a);
        delete session;
        const DigestEngine::Digest &digest = md5.digest();
        return DigestEngine::digestToHex(digest);
    }
    catch (std::exception &e){
        root.error(e.what());
        delete session;
        throw;
    }
}
