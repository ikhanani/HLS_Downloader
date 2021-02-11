cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
get_filename_component(PROJECT_ID ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_ID})
set(MAINFOLDER ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MAINFOLDER}/bin")
set(LIBRARY_OUTPUT_PATH "${MAINFOLDER}/lib")
add_subdirectory(src)
add_subdirectory(test)


