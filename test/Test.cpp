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

TEST(FetcherTest, fetch){
  /**string baseUrl = "http://localhost:9980";
  concurrency::streams::container_buffer<std::vector<uint8_t>> buffer;
  VideoFetcher fetcher = VideoFetcher("http://localhost:9980", "step1");
  concurrency::streams::ostream outfile(buffer);
  fetcher.fetch(&outfile, "http://localhost:9980/step1");
  vector<uint8_t> data = buffer.collection();
  string str(data.begin(), data.end());
  //cout << str;
  string ans = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-STREAM-INF:BANDWIDTH=440283,CODECS= avc1.77.20 ,RESOLUTION=352x240\nstep2";
  Converter c;
  unique_ptr<ChunkList> chunk = c.chunkConverter(buffer.collection());
  concurrency::streams::container_buffer<std::vector<uint8_t>> buffer2;
  concurrency::streams::ostream outfile2(buffer2);
  cout << chunk->getChunkName();
  fetcher.fetch(&outfile2, baseUrl + "/" + chunk->getChunkName());
  vector<uint8_t> data2 = buffer2.collection();
  string str2(data2.begin(), data2.end());
  string ans2 = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-ALLOW-CACHE:NO\n#EXT-X-TARGETDURATION:11\n#EXT-X-MEDIA-SEQUENCE:1010\n#EXTINF:10.01,\nfile1\n#EXTINF:10.01,\nfile2";
  unique_ptr<PlayList> play = c.playListConverter(buffer2.collection());
  ASSERT_EQ(str, ans);
  ASSERT_EQ(str2, ans2);**/
  VideoFetcher fetcher = VideoFetcher("http://[::1]:9980", "step1");
  std::unique_ptr<PlayList> play = fetcher.fetchPlayList();
  ASSERT_EQ(play->getFiles().at(0)->getName(), "file1");

}
TEST(ChunkTest, chunk){
    string content = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-STREAM-INF:BANDWIDTH=440283,CODECS= avc1.77.20 ,RESOLUTION=352x240\nchunklist_w1215933555.m3u8";
    Converter c;
    std::unique_ptr<ChunkList> test = c.chunkConverter(content);
    ASSERT_EQ(test->getVersion(), 3);
    ASSERT_EQ(test->getBandwidth(), 440283);
    ASSERT_EQ(test->getCodecs(),"avc1.77.20");
    ASSERT_EQ(test->getResolution(), "352x240");
    ASSERT_EQ(test->getChunkName(), "chunklist_w1215933555.m3u8");
    


}
TEST(PlayTest, play){
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
TEST(ConfigTest, config){

  hls::Config c;
  c.read("{ \n \"enableLocal\" : true,\n \"baseDir\" : \"/tmp\",\n \"urls\" : [\"https://itsvideo.arlingtonva.us:8013/live/cam257.stream\", \"https://itsvideo.arlingtonva.us:8013/live/cam258.stream\", \"https://itsvideo.arlingtonva.us:8013/live/cam259.stream\"]\n}");
  ASSERT_EQ(c.getDir(), "/tmp");
  ASSERT_EQ(c.getEnableLocal(), true);
  ASSERT_EQ(c.getUrls().size(), 3);
  ASSERT_EQ(c.getUrls().at(0), "https://itsvideo.arlingtonva.us:8013/live/cam257.stream");
  ASSERT_EQ(c.getUrls().at(1), "https://itsvideo.arlingtonva.us:8013/live/cam258.stream");
  ASSERT_EQ(c.getUrls().at(2), "https://itsvideo.arlingtonva.us:8013/live/cam259.stream");
}
TEST(PathTest, path){
  MediaFile f;
  boost::filesystem::path p = f.getPath("https://itsvideo.arlingtonva.us:8013/live/cam259.stream");
  ASSERT_EQ(p.string(), "itsvideo.arlingtonva.us:8013/live/cam259.stream");
}
int main(int argc, char **argv) {
  auto f = [](int argc, char **argv){
    HLSTestServer app;
    app.run(argc, argv);
  };
  std::thread thread_object(f, argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}