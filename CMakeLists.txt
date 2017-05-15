cmake_minimum_required(VERSION 2.7)
project(Lettrine)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(sources/bloblib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -pedantic -std=c++14")

set(SOURCE_FILES
        sources/main.cpp
        sources/extractor.cpp
        sources/nbt.hpp
        sources/bloblib/blob.cpp
        sources/bloblib/blob.h
        sources/bloblib/BlobContour.cpp
        sources/bloblib/BlobContour.h
        sources/bloblib/BlobLibraryConfiguration.h
        sources/bloblib/BlobOperators.cpp
        sources/bloblib/BlobOperators.h
        sources/bloblib/BlobResult.cpp
        sources/bloblib/BlobResult.h
        sources/bloblib/ComponentLabeling.cpp
        sources/bloblib/ComponentLabeling.h)

add_executable(Lettrine ${SOURCE_FILES})
target_link_libraries( Lettrine ${OpenCV_LIBS} boost_system boost_filesystem pthread)

add_subdirectory(gtest)
add_subdirectory(UnitTests)

enable_testing()
add_test(NAME Lettrine_Tests COMMAND Lettrine_Tests)