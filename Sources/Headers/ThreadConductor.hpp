//
// Created by aguadoe on 18/05/17.
//

#ifndef LETTRINE_THREADCONDUCTOR_HPP
#define LETTRINE_THREADCONDUCTOR_HPP

#include <mutex>
#include <queue>
#include <thread>
#include <Extractor.hpp>

void processImage(std::queue<std::tuple<std::string, std::string>> &files);
void startProcessThreads(std::queue<std::tuple<std::string, std::string>> &files);

#endif //LETTRINE_THREADCONDUCTOR_HPP
