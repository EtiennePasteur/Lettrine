cmake_minimum_required(VERSION 3.7)
project(Lettrine)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(bloblib)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        ShapeDetector/main.cpp
        bloblib/blob.cpp
        bloblib/blob.h
        bloblib/BlobContour.cpp
        bloblib/BlobContour.h
        bloblib/BlobLibraryConfiguration.h
        bloblib/BlobOperators.cpp
        bloblib/BlobOperators.h
        bloblib/BlobResult.cpp
        bloblib/BlobResult.h
        bloblib/ComponentLabeling.cpp
        bloblib/ComponentLabeling.h
        )

add_executable(Lettrine ${SOURCE_FILES})
target_link_libraries( Lettrine ${OpenCV_LIBS} )