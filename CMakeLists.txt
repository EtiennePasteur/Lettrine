cmake_minimum_required(VERSION 3.7)
project(Lettrine)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(image_extractor/bloblib)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        image_extractor/extractor.cpp
        image_extractor/bloblib/blob.cpp
        image_extractor/bloblib/blob.h
        image_extractor/bloblib/BlobContour.cpp
        image_extractor/bloblib/BlobContour.h
        image_extractor/bloblib/BlobLibraryConfiguration.h
        image_extractor/bloblib/BlobOperators.cpp
        image_extractor/bloblib/BlobOperators.h
        image_extractor/bloblib/BlobResult.cpp
        image_extractor/bloblib/BlobResult.h
        image_extractor/bloblib/ComponentLabeling.cpp
        image_extractor/bloblib/ComponentLabeling.h
        )

add_executable(Lettrine ${SOURCE_FILES})
target_link_libraries( Lettrine ${OpenCV_LIBS} )