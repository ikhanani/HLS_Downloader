#include <gtest/gtest.h>
#include <string>
#include "../src/Converter.h"
#include "../src/VideoFetcher.h"
#include <fstream>
#include <cpprest/filestream.h>
#include <cpprest/interopstream.h>
#include <sstream>
#include <vector>
#include "../src/Config.h"


TEST(ChunkTest, chunk){
    string content = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-STREAM-INF:BANDWIDTH=440283,CODECS= avc1.77.20 ,RESOLUTION=352x240\nchunklist_w1215933555.m3u8";
    Converter c;
    vector<uint8_t> vec(content.begin(), content.end());
    std::unique_ptr<ChunkList> test = c.chunkConverter(vec);
    ASSERT_EQ(test->getVersion(), 3);
    ASSERT_EQ(test->getBandwidth(), 440283);
    ASSERT_EQ(test->getCodecs(),"avc1.77.20");
    ASSERT_EQ(test->getResolution(), "352x240");
    ASSERT_EQ(test->getChunkName(), "chunklist_w1215933555.m3u8");
    


}
TEST(PlayTest, play){
  string content = "#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-ALLOW-CACHE:NO\n#EXT-X-TARGETDURATION:11\n#EXT-X-MEDIA-SEQUENCE:1010\n#EXTINF:10.01,\nmedia_w604857604_1010.ts\n#EXTINF:10.01,\nmedia_w604857604_1011.ts";
  Converter c;
  vector<uint8_t> vec(content.begin(), content.end());
  std::unique_ptr<PlayList> test = c.playListConverter(vec);
  ASSERT_EQ(test->getVersion(), 3);
  ASSERT_EQ(test->getAllowCache(), false);
  ASSERT_EQ(test->getTargetDuration(), 11);
  ASSERT_EQ(test->getMediaSequence(), 1010);
  vector<MediaFile> files = test->getFiles();
  ASSERT_EQ(files.size(), 2);
  ASSERT_EQ(files[0].getName(), "media_w604857604_1010.ts");
  ASSERT_EQ(files[0].getDuration(), 10.01);
  ASSERT_EQ(files[1].getName(), "media_w604857604_1011.ts");

}
TEST(ConfigTest, config){

  Config c;
  c.read("{ \n \"enableLocal\" : true,\n \"baseDir\" : \"/tmp\",\n \"urls\" : [\"https://itsvideo.arlingtonva.us:8013/live/cam257.stream\", \"https://itsvideo.arlingtonva.us:8013/live/cam258.stream\", \"https://itsvideo.arlingtonva.us:8013/live/cam259.stream\"]\n}");
  ASSERT_EQ(c.getDir(), "/tmp");
  ASSERT_EQ(c.getEnableLocal(), true);
  ASSERT_EQ(c.getUrls().size(), 3);
  ASSERT_EQ(c.getUrls().at(0), "https://itsvideo.arlingtonva.us:8013/live/cam257.stream");
  ASSERT_EQ(c.getUrls().at(1), "https://itsvideo.arlingtonva.us:8013/live/cam258.stream");
  ASSERT_EQ(c.getUrls().at(2), "https://itsvideo.arlingtonva.us:8013/live/cam259.stream");
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}