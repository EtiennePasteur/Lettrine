// std and boost utils
#include <iostream>
#include <boost/format.hpp>
template<typename... Args>
constexpr boost::basic_format<char> fmt(Args &&... args) {
    return (boost::format(std::forward<Args>(args)...));
}
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <thread>
#include <queue>
#include <mutex>

namespace fs = boost::filesystem;

// project includes
#include "nbt.hpp"
void extractPics(const std::string &path, const std::string &destPath);

static char const *const _version = "0.1.0";
static char const *const _usage =
        R"(Usage: %s SOURCE

	SOURCE		url or directory


Lettrine %s - Open Library Hackathon 2017)";

static inline int usage(char const *const name) {
    std::cout << fmt(_usage) % name % _version << std::endl;
    return (1);
}

std::mutex _lock;

void processImage(std::queue<std::tuple<std::string, std::string>> &files) {
    // Check if some work is still available in the queue
    while (true) {
        _lock.lock();
        if (files.size() == 0) {
            _lock.unlock();
            return;
        }

        auto &file = files.front();

        std::string path{std::get<0>(file)};
        std::string filename{std::get<1>(file)};
        files.pop();
        std::cout << "Remaining files to parse : " << files.size();

        _lock.unlock();

        std::cout << " [" << path << "]" << std::endl;
        extractPics(path, filename);
    }
}

void startProcessThreads(std::queue<std::tuple<std::string, std::string>> &files) {
    std::vector<std::thread> threadPool;
    auto threadNumber = std::thread::hardware_concurrency();

    for (unsigned int i = 0; i < threadNumber; ++i) {
        // Thread does not take rvalues, so we wrap the reference into a copyable object
        threadPool.push_back(std::thread(processImage, std::ref(files))); // Implicit call to move constructor
    }

    for (auto &i : threadPool) {
        i.join();
    }
}

int main(int ac, char **av) {
    if (ac != 2)
        return (usage(av[0]));

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
                    files.push({entry.path().string(), file + "_%02d.jpg"});
                }
            }
        }
    }
    startProcessThreads(files);
    return (0);
}
