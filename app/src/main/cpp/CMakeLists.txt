# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("ffmpegedu")

set(HEADER ${PROJECT_SOURCE_DIR}/includes)
MESSAGE(${HEADER})
include_directories(${HEADER})

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        ffmpegedu

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp
        X264Encoder.cpp X264Encoder.h native_ffmpeg.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

set(SO_DIR ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
#set(SO_DIR ${ANDROID_ABI})
MESSAGE(${CMAKE_SOURCE_DIR})

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libavcodec.so)
add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libavdevice.so)
add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libavfilter.so)
add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libavformat.so)
add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libavutil.so)
add_library(postproc SHARED IMPORTED)
set_target_properties(postproc PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libpostproc.so)
add_library(swresample SHARED IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libswresample.so)
add_library(swscale SHARED IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libswscale.so)

add_library(fdk-aac SHARED IMPORTED)
set_target_properties(fdk-aac PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libfdk-aac.so)

add_library(x264 SHARED IMPORTED)
set_target_properties(x264 PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libx264.a)

add_library(lame SHARED IMPORTED)
set_target_properties(lame PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libmp3lame.so)

add_library(ssl SHARED IMPORTED)
set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libssl.so)

add_library(crypto SHARED IMPORTED)
set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${SO_DIR}/libcrypto.so)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        ffmpegedu
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        postproc
        swresample
        swscale
        fdk-aac
        x264
        lame
        ssl
        crypto
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib}
        android)