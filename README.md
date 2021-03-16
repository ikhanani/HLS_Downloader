# HLS_Downloader
HLS Downloader is a C++ application that downloads media files from servers that use the HTTP Live Streaming (HLS) Protocol.
An example use is accessing and saving footage from desired traffic cameras in Arlington, Virginia. The application can also upload directly to AWS S3 buckets. This could be useful for potential machine learning applications.

Specific camera links can be plugged into the configuration file (explained [below](#configuration))

[County of Arlington, Virginia](https://transportation.arlingtonva.us/live-traffic-cameras/)
#### Example Camera Links
| Camera Location | URL |
| ----------- | ----------- |
| Glebe Road at Carlin Springs | https://itsvideo.arlingtonva.us:8013/live/cam181.stream/playlist.m3u8 |
| Route 50 @ Patrick Henry | https://itsvideo.arlingtonva.us:8013/live//cam23.stream/playlist.m3u8 |
#### State Websites
[Virginia Department of Transportation](https://www.511virginia.org/)
| Camera Location | URL |
| ----------- | ----------- |
| I-95 / MM 138.6 / SB |https://s16.us-east-1.skyvdn.com/rtplive/FairfaxVideo2065/playlist.m3u8 |
| US-250 / WB / SR-20 / Charlottesville | https://s11.us-east-1.skyvdn.com/rtplive/StauntonCCTV250PAMTOPE/playlist.m3u8 |

[Maryland Department of Transportation](https://chart.maryland.gov/map/default.asp)
| Camera Location | URL |
| ----------- | ----------- |
| Paint Branch Pkwy at MFRI |https://strmr10.sha.maryland.gov/rtplive/c5ff65d2008900a1004f823633235daa/playlist.m3u8|
| RWIS I-95 AT PATUXENT RIVER | https://strmr3.sha.maryland.gov/rtplive/0301bd3e006700d40050fa36c4235c0a/playlist.m3u8 |
## Dependencies:
* AWSSDK
    * Clone repository with git: 
    ```
    git clone https://github.com/aws/aws-sdk-cpp.git
    mkdir sdk_build
    sudo cmake -DCMAKE_BUILD_TYPE=[Release] -DBUILD_ONLY="s3"
    make install
    ```
* Poco 
    * Clone repository with git: 
    ```
    $ git clone -b master https://github.com/pocoproject/poco.git
    $ cd poco
    $ mkdir cmake-build
    $ cd cmake-build
    $ cmake ..
    $ sudo cmake --build . --target install
    ```

* OpenSSL
* Boost
    * Download from https://www.boost.org/
* GTest
    * Clone repository with git:
    ```
    git clone https://github.com/google/googletest.git -b release-1.10.0
    cd googletest        # Main directory of the cloned repository.
    mkdir build          # Create a directory to hold the build output.
    cd build
    cmake ..             # Generate native build scripts for GoogleTest.
    make
    sudo make install    # Install in /usr/local/ by default
    ```
## Design

## How To Run the Application
When you open the Udacity Virtual Machine, the required packages will already be downloaded. However, you will still need to run "make install" in each build directory as shown below. The default configurations have AWS off, but you may enable it if you have your own credentials.

##### Aws build
![](../HLS_Downloader/HLSGifs/AwsInstall.gif)
##### Poco Build
![](../HLS_Downloader/HLSGifs/PocoInstall.gif)
##### GTeest Build
![](../HLS_Downloader/HLSGifs/GoogleInstall.gif)

You will now need to open Visual Studio and open the project folder. Open the CMakeLists file and install CMakeTools as the pop up suggests. Configure the project.
![](../HLS_Downloader/HLSGifs/CMakeTools.gif)
You will need to select a compiler. Select CLang6.0.1.
![](../HLS_Downloader/HLSGifs/CLang.gif)

Navigate to the terminal and build the project.
![](../HLS_Downloader/HLSGifs/CMake.gif)
![](../HLS_Downloader/HLSGifs/Make.gif)

You can now run the project as shown below:
![](../HLS_Downloader/HLSGifs/RunMain.gif)

Allow the projct to run as long as you would like. The longer it runs, the more footage it saves.

Under the bin directory you will see a path that leads to the video files.
![](../HLS_Downloader/HLSGifs/Result.gif)
## How To Run the Tests
You can run the tests by running the HLS_Downloader_test executable. The program creates its own server to run the HTTP tests.
![](../HLS_Downloader/HLSGifs/Test.gif)
## Configuration
Under the /config directory, there is a file named "config.json". The following variables can be modified depending on how you would like the program to run.
* enableLocal - Determines whether or not to save media files locally
* baseDir - Sets base directoy
* urls - List of urls for each traffic camera to access
* level - Sets level for logger
* format - Sets format for logger
* rotation - Sets max file size for log files
* cacheSize - Sets size of cache for duplicate file detection
* enableAws - Determines whether or not to save media files to Amazon Web Services (set to false if you do not have an AWS key)
* bucketName - Name of s3 bucket
* AwsRegion - Region of AWS bucket
* delay - Time between executions of main loop in milliseconds. Note: A delay that is too large may result in missed media files

## Rubric Items
                              
|Criteria| File(s)| Line|
|--------|-----|-----|
|"The project uses Object Oriented Programming techniques."|  HLSObjects.h, VideoFetcher.cpp| All|
|"Classes use appropriate access specifiers for class members."| HLSObjects.h, Converter.h, FilesystemPersister.h, Job.h| All|
|"The project reads data from a file and process the data, or the program writes data to a file."| VideoFetcher.cpp| 24|
|"Derived class functions override virtual base class functions." | FilesystemPersister.cpp| 7|
|"The project uses smart pointers instead of raw pointers."| Job.cpp |19
|"The project uses multithreading."| Driver.cpp| 74
|"A mutex or lock is used in the project."| Test.cpp| 93|
|"The project makes use of references in function declarations."| HLSObjects.cpp, VideoFetcher.cpp| 4, 24|
|"A condition variable is used in the project."|Test.cpp| 85|