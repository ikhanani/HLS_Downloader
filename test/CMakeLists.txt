set(BINARY ${PROJECT_NAME}_test)
find_package(GTest REQUIRED)
find_package(Poco REQUIRED Net Util JSON)
#set(SOURCES ${TEST_SOURCES})
include_directories(${GTEST_INCLUDE_DIRS})
file(GLOB SOURCES *.h *.cpp)

add_executable(${BINARY} ${SOURCES})

#add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC Poco::Net Poco::Util Poco::JSON HLS_Downloader_LIB ${GTEST_LIBRARIES})

