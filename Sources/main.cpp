//
// Created by Etienne Pasteur on 13/05/2017.
//

#include <iostream>
#include <boost/format.hpp>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "Extractor.hpp"
#include "ThreadConductor.hpp"

template<typename... Args>
constexpr auto fmt(Args &&... args) {
    return (boost::format(std::forward<Args>(args)...));
}

namespace fs = boost::filesystem;

int main(int ac, char **av) {
    std::string dirName = "";
    std::string fileName = "";
    std::string outputDirName = "";

    try {
        TCLAP::CmdLine cmd("Image Extraction from Digitised Books", ' ', "1.0");
        TCLAP::ValueArg<std::string> dirArg("d", "directory", "Directory to scan", false, "", "directory name");
        TCLAP::ValueArg<std::string> fileArg("f", "file", "File to scan", false, "", "file name");
        TCLAP::ValueArg<std::string> outputArg("o", "output-dir", "Output directory name", false, "extracted", "output directory name");

        cmd.xorAdd(dirArg, fileArg);
        cmd.add(outputArg);
        cmd.parse(ac, av);
        dirName = dirArg.getValue();
        fileName = fileArg.getValue();
        outputDirName = outputArg.getValue();
    } catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }

    std::queue<std::tuple<std::string, std::string>> files;

    if (dirName != "") {
        fs::path imgDir(dirName);
        if (fs::is_directory(imgDir)) {
            std::cout << fmt("Entering %s directory...") % dirName << std::endl;

            std::string file;
            std::string outName = imgDir.string() + "/" + outputDirName + "/";
            fs::path outDir(outName);
            fs::create_directory(outName);
            for (auto &&entry : boost::make_iterator_range(fs::directory_iterator(imgDir), {})) {
                if (entry.path().extension() == ".jpg") {
                    file = outDir.string() + entry.path().filename().string();
                    file.erase(file.find_last_of("."), std::string::npos);
                    files.push(std::tuple<std::string, std::string>(entry.path().string(), file + "_%02d.jpg"));
                }
            }
        }
    } else if (fileName != "") {
        std::string file;
        fs::path img(fileName);
        if (fs::exists(img)) {
            std::string outName = img.parent_path().string() + "/" + outputDirName + "/";
            fs::path outDir(outName);
            fs::create_directory(outName);
            if (img.extension() == ".jpg") {
                file = outDir.string() + img.filename().string();
                file.erase(file.find_last_of("."), std::string::npos);
                files.push(std::tuple<std::string, std::string>(img.string(), file + "_%02d.jpg"));
            }
        }
    }
    if (!files.empty())
        startProcessThreads(files);
    return (0);
}
