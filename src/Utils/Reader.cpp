#include <utility>

//
// Created by WF on 2018/9/6.
//

#include "Reader.h"

Reader::Reader(std::string path) {
    filePath = std::move(path);
    if (!file) {
        file = std::ifstream(filePath);
    }
}

std::string Reader::getNextChar() {
    std::string cur = nullptr;
    if (!file.is_open()) {
        std::cout << "Error opening file";
    }
    if (!file.eof()) {
        file >> cur;
    }
    return cur;
}

Reader::~Reader() {
    if (file.is_open()) {
        file.close();
    }
}
