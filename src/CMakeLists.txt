#set(-std=c++11 -lcpprest -lssl -lcrypto -lboost_system -lboost_thread-mt -lboost_chrono-mt -L/usr/local/Cellar/openssl\@1.1/1.1.1i/lib/)
option(BUILD_SHARED_LIBS "" ON)
set(project_BIN ${PROJECT_NAME})
set(project_LIB ${PROJECT_NAME}_LIB)
set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
#set(spdlog_DIR "/dev/spdlog")
set(CMAKE_PREFIX_PATH, /usr/lib/x86_64-linux-gnu/cmake)
find_package(AWSSDK REQUIRED COMPONENTS s3)
find_package(OpenSSL REQUIRED)
find_package(Poco REQUIRED Net Util JSON NetSSL)
file(GLOB SOURCES *.h *.cpp)
add_library(${project_LIB} ${SOURCES})
add_executable(${project_BIN} ${SOURCES})
find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(${project_LIB} ${Boost_FILESYSTEM_LIBRARY})
target_link_libraries(${project_LIB} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(${project_LIB} ${OPENSSL_LIBRARIES} ${AWSSDK_LINK_LIBRARIES})
target_link_libraries(${project_LIB} Poco::Net Poco::Util Poco::JSON Poco::NetSSL)
target_link_libraries(${project_BIN} ${project_LIB})
