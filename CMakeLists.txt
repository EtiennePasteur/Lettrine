cmake_minimum_required(VERSION 3.7)
project(Lettrine)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        ShapeDetector/main.cpp)

add_executable(Lettrine ${SOURCE_FILES})
target_link_libraries( Lettrine ${OpenCV_LIBS} )