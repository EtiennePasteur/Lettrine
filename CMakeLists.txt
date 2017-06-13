cmake_minimum_required(VERSION 2.7)
project(Lettrine)

find_package( OpenCV REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(Sources/libs)
include_directories(Sources/libs/bloblib)
include_directories(Sources/Headers)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -pedantic -std=c++14")

set(TCLAP_FILES
        Sources/libs/tclap/CmdLineInterface.h
        Sources/libs/tclap/ArgException.h
        Sources/libs/tclap/CmdLine.h
        Sources/libs/tclap/XorHandler.h
        Sources/libs/tclap/MultiArg.h
        Sources/libs/tclap/UnlabeledMultiArg.h
        Sources/libs/tclap/ValueArg.h
        Sources/libs/tclap/UnlabeledValueArg.h
        Sources/libs/tclap/Visitor.h
        Sources/libs/tclap/Arg.h
        Sources/libs/tclap/HelpVisitor.h
        Sources/libs/tclap/SwitchArg.h
        Sources/libs/tclap/MultiSwitchArg.h
        Sources/libs/tclap/VersionVisitor.h
        Sources/libs/tclap/IgnoreRestVisitor.h
        Sources/libs/tclap/CmdLineOutput.h
        Sources/libs/tclap/StdOutput.h
        Sources/libs/tclap/DocBookOutput.h
        Sources/libs/tclap/ZshCompletionOutput.h
        Sources/libs/tclap/OptionalUnlabeledTracker.h
        Sources/libs/tclap/Constraint.h
        Sources/libs/tclap/ValuesConstraint.h
        Sources/libs/tclap/ArgTraits.h
        Sources/libs/tclap/StandardTraits.h
        )

set(BLOB_FILES
        Sources/libs/bloblib/blob.cpp
        Sources/libs/bloblib/blob.h
        Sources/libs/bloblib/BlobContour.cpp
        Sources/libs/bloblib/BlobContour.h
        Sources/libs/bloblib/BlobLibraryConfiguration.h
        Sources/libs/bloblib/BlobOperators.cpp
        Sources/libs/bloblib/BlobOperators.h
        Sources/libs/bloblib/BlobResult.cpp
        Sources/libs/bloblib/BlobResult.h
        Sources/libs/bloblib/ComponentLabeling.cpp
        Sources/libs/bloblib/ComponentLabeling.h
        )

set(SOURCE_FILES
        Sources/main.cpp
        Sources/Extractor.cpp
        Sources/Headers/Extractor.hpp
        Sources/ThreadConductor.cpp
        Sources/Headers/ThreadConductor.hpp
        )

add_executable(Lettrine ${TCLAP_FILES} ${BLOB_FILES} ${SOURCE_FILES})
target_link_libraries( Lettrine ${OpenCV_LIBS} boost_system boost_filesystem pthread)

add_subdirectory(gtest)
add_subdirectory(UnitTests)

enable_testing()
add_test(NAME Lettrine_Tests COMMAND Lettrine_Tests)