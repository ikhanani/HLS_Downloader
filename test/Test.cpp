#include <gtest/gtest.h>
#include <string>
#include <thread>
#include "../src/Converter.h"
#include "../src/VideoFetcher.h"
#include "../test/HLSTestServer.cpp"
#include <fstream>
#include <sstream>
#include <vector>
#include "../src/Config.h"
#include "../src/VideoCache.h"
#include <condition_variable>

TEST(ChunkTest, chunk) {
    string content = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-STREAM-INF:BANDWIDTH=440283,CODECS= avc1.77.20 ,RESOLUTION=352x240\nchunklist_w1215933555.m3u8";
    Converter c;
    std::unique_ptr<ChunkList> test = c.chunkConverter(content);
    ASSERT_EQ(test->getVersion(), 3);
    ASSERT_EQ(test->getBandwidth(), 440283);
    ASSERT_EQ(test->getCodecs(), "avc1.77.20");
    ASSERT_EQ(test->getResolution(), "352x240");
    ASSERT_EQ(test->getChunkName(), "chunklist_w1215933555.m3u8");
}

TEST(PlayTest, play) {
    string content = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-ALLOW-CACHE:NO\n#EXT-X-TARGETDURATION:11\n#EXT-X-MEDIA-SEQUENCE:1010\n#EXTINF:10.01,\nmedia_w604857604_1010.ts\n#EXTINF:10.01,\nmedia_w604857604_1011.ts";
    Converter c;
    std::unique_ptr<PlayList> test = c.playListConverter(content);
    ASSERT_EQ(test->getVersion(), 3);
    ASSERT_EQ(test->getAllowCache(), false);
    ASSERT_EQ(test->getTargetDuration(), 11);
    ASSERT_EQ(test->getMediaSequence(), 1010);
    vector<std::shared_ptr<MediaFile>> files = test->getFiles();
    ASSERT_EQ(files.size(), 2);
    ASSERT_EQ(files[0]->getName(), "media_w604857604_1010.ts");
    ASSERT_EQ(files[0]->getDuration(), 10.01);
    ASSERT_EQ(files[1]->getName(), "media_w604857604_1011.ts");

}

TEST(ConfigTest, config) {

    hls::Config c;
    c.read("{ \n \"enableLocal\" : true,\n \"baseDir\" : \"/tmp\",\n \"urls\" : [\"https://itsvideo.arlingtonva.us:8013/live/cam257.stream\", \"https://itsvideo.arlingtonva.us:8013/live/cam258.stream\", \"https://itsvideo.arlingtonva.us:8013/live/cam259.stream\"],\n \"level\" : \"INFORMATION\",\n\"format\" : \"%Y-%m-%d %H:%M:%S %s: %t\",\n\"rotation\" : \"2 K\",\n\"archive\" : \"number\",\"cacheSize\" : \"100\",\n \"enableAws\" : true,\n \"bucketName\" : \"hls-dataset\", \n  \"AwsRegion\" : \"us-east-1\", \n \"delay\" : 1000\n}");
    ASSERT_EQ(c.getDir(), "/tmp");
    ASSERT_EQ(c.getEnableLocal(), true);
    ASSERT_EQ(c.getUrls().size(), 3);
    ASSERT_EQ(c.getUrls().at(0), "https://itsvideo.arlingtonva.us:8013/live/cam257.stream");
    ASSERT_EQ(c.getUrls().at(1), "https://itsvideo.arlingtonva.us:8013/live/cam258.stream");
    ASSERT_EQ(c.getUrls().at(2), "https://itsvideo.arlingtonva.us:8013/live/cam259.stream");
}

TEST(PathTest, path) {
    MediaFile f;
    time_t theTime = time(0);
    boost::filesystem::path p = f.getPath("https://itsvideo.arlingtonva.us:8013/live/cam259.stream", theTime);
    struct tm *aTime = localtime(&theTime);
    ASSERT_EQ(p.string(),
              "itsvideo.arlingtonva.us:8013/live/cam259.stream/" + std::to_string(aTime->tm_year + 1900) + "/" +
              std::to_string(aTime->tm_mon + 1) + "/" + std::to_string(aTime->tm_mday));
}

TEST(FetcherTest, fetch) {
    VideoFetcher fetcher = VideoFetcher("http://127.0.0.1:9980", "step1");
    std::unique_ptr<PlayList> play = fetcher.fetchPlayList();
    ASSERT_EQ(play->getFiles().size(), 2);
    ASSERT_EQ(play->getFiles().at(0)->getName(), "file1");
    ASSERT_EQ(play->getFiles().at(1)->getName(), "file2");

}

TEST(HashTest, hash) {
    VideoFetcher fetcher = VideoFetcher("http://127.0.0.1:9980", "step1");
    std::unique_ptr<PlayList> play = fetcher.fetchPlayList();
    string hash = fetcher.fetch(std::cout, "http://127.0.0.1:9980/" + play->getFiles()[0]->getName());
    ASSERT_EQ(hash, "d41d8cd98f00b204e9800998ecf8427e");
}

TEST(CacheTest, cache) {
    VideoCache cache(1);
    bool duplicate;
    duplicate = cache.checkFile("https://itsvideo.arlingtonva.us:8013/live/cam259.stream",
                                "cdb239db254c3328e38617fd0fd1afab");
    duplicate = cache.checkFile("https://itsvideo.arlingtonva.us:8013/live/cam259.stream",
                                "cdb239db254c3328e38617fd0fd1afab");
    ASSERT_TRUE(duplicate);
    duplicate = cache.checkFile("https://itsvideo.arlingtonva.us:8013/live/cam259.stream",
                                "d41d8cd98f00b204e9800998ecf8427e");
    ASSERT_FALSE(duplicate);
    duplicate = cache.checkFile("https://itsvideo.arlingtonva.us:8013/live/cam259.stream",
                                "d41d8cd98f00b204e9800998ecf8427e");
    duplicate = cache.checkFile("https://itsvideo.arlingtonva.us:8013/live/cam259.stream",
                                "cdb239db254c3328e38617fd0fd1afab");
    ASSERT_FALSE(duplicate);
}

int main(int argc, char **argv) {
    std::condition_variable cv;
    std::mutex m;
    bool ready = false;
    auto f = [&](int argc, char **argv) {
        HLSTestServer app(&m, &cv, &ready);
        app.run(argc, argv);
    };
    std::thread thread_object(f, argc, argv);
    std::unique_lock<mutex> ul(m);
    cout << "Lock in main" << "\n";
    cv.wait(ul, [&] { return ready; });
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 