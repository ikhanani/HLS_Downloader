set(BINARY ${PROJECT_NAME}_test)
find_package(GTest REQUIRED)
#set(SOURCES ${TEST_SOURCES})
include_directories(${GTEST_INCLUDE_DIRS})
file(GLOB SOURCES *.h *.cpp)

add_executable(${BINARY} ${SOURCES})

#add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} HLS_Downloader_LIB ${GTEST_LIBRARIES})
