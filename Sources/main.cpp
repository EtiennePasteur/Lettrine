// std and boost utils
#include <iostream>
#include <boost/format.hpp>
template<typename... Args>
constexpr auto fmt(Args &&... args) {
    return (boost::format(std::forward<Args>(args)...));
}
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "Extractor.hpp"
#include "ThreadConductor.hpp"

namespace fs = boost::filesystem;


static char const *const _version = "0.1.0";
static char const *const _usage =
        R"(Usage: %s SOURCE

	SOURCE		url or directory


Lettrine %s - Open Library Hackathon 2017)";

static inline int usage(char const *const name) {
    std::cout << fmt(_usage) % name % _version << std::endl;
    return (1);
}

int main(int ac, char **av) {
    if (ac != 2)
        return (usage(av[0]));

    std::cout << "OpenCV is running in " << (cv::useOptimized() ? "optimized" : "non-optimized") << " mode" << std::endl;

    std::queue<std::tuple<std::string, std::string>> files;

    fs::path const p(av[1]);
    if (fs::is_directory(p)) {
        fs::path dir;
        fs::path img;
        std::string file;
        //Socc socc("4242");

        // TODO: Only handle image files
        // TODO: Handle multiple directory structures
        // MAYBE TODO: Accept more than JPG

        std::cout << fmt("Entering %s directory...") % av[1] << std::endl;
        for (auto &&entry : boost::make_iterator_range(fs::directory_iterator(p), {})) {
            dir = entry.path();
            file = (fmt("%s/metadata/%s.json") % dir.string() % dir.filename().string()).str();
            if (!fs::is_directory(dir) || !fs::is_regular_file(file))
                continue;
            std::cout << fmt("loading %s ...\n") % file << std::endl;
            //socc.send(file);
            img = fs::path(dir.string() + "/img");
            if (fs::is_directory(img)) {
                fs::create_directory((dir += "/pics/"));
                for (auto &&entry : boost::make_iterator_range(fs::directory_iterator(img), {})) {
                    file = dir.string() + entry.path().filename().string();
                    file.erase(file.find_last_of("."), std::string::npos);
                    files.push(std::tuple<std::string, std::string>(entry.path().string(), file + "_%02d.jpg"));
                }
            }
        }
    }
    startProcessThreads(files);
    return (0);
}
