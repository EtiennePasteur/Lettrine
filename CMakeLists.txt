cmake_minimum_required(VERSION 2.7)
project(Lettrine)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(Sources/bloblib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -pedantic -std=c++14")

set(SOURCE_FILES
        Sources/main.cpp
        Sources/extractor.cpp
        Sources/nbt.hpp
        Sources/bloblib/blob.cpp
        Sources/bloblib/blob.h
        Sources/bloblib/BlobContour.cpp
        Sources/bloblib/BlobContour.h
        Sources/bloblib/BlobLibraryConfiguration.h
        Sources/bloblib/BlobOperators.cpp
        Sources/bloblib/BlobOperators.h
        Sources/bloblib/BlobResult.cpp
        Sources/bloblib/BlobResult.h
        Sources/bloblib/ComponentLabeling.cpp
        Sources/bloblib/ComponentLabeling.h Sources/ThreadConductor.cpp)

add_executable(Lettrine ${SOURCE_FILES})
target_link_libraries( Lettrine ${OpenCV_LIBS} boost_system boost_filesystem pthread)

add_subdirectory(gtest)
add_subdirectory(UnitTests)

enable_testing()
add_test(NAME Lettrine_Tests COMMAND Lettrine_Tests)