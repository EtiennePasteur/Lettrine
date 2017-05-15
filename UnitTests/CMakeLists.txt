cmake_minimum_required(VERSION 2.7)
project(Lettrine_Tests)

find_package( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -pedantic -std=c++14")

set(SOURCE_FILES
        DummyTest.cpp)

add_executable(Lettrine_Tests ${SOURCE_FILES})

target_include_directories(Lettrine_Tests PUBLIC ${CMAKE_SOURCE_DIR}/gtest/googletest ${CMAKE_SOURCE_DIR}/gtest/googletest/include ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/sources/bloblib/)
target_link_libraries(Lettrine_Tests gtest gtest_main ${OpenCV_LIBS} boost_system boost_filesystem pthread)