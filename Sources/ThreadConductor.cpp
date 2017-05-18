//
// Created by aguadoe on 18/05/17.
//


#include "ThreadConductor.hpp"

std::mutex _lock;

void processImage(std::queue<std::tuple<std::string, std::string>> &files) {
    while (true) {
        // Check if some work is still available in the queue
        _lock.lock();
        if (files.size() == 0) {
            _lock.unlock();
            return;
        }

        auto &file = files.front();
        std::string path{std::get<0>(file)};
        std::string filename{std::get<1>(file)};
        files.pop();

        std::cout << "Pages left : " << files.size() << "  |  current : " << path << std::endl;

        _lock.unlock();

        extractPics(path, filename);
    }
}

void startProcessThreads(std::queue<std::tuple<std::string, std::string>> &files) {
    std::vector<std::thread> threadPool;
    auto threadNumber = std::thread::hardware_concurrency() || 1;

    for (unsigned int i = 0; i < threadNumber; ++i) {
        // Thread does not take rvalues, so we wrap the reference into a copyable object
        threadPool.push_back(std::thread(processImage, std::ref(files))); // Implicit call to move constructor
    }

    for (auto &i : threadPool) {
        i.join();
    }
}