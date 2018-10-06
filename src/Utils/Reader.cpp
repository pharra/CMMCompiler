#include <utility>

//
// Created by WF on 2018/9/6.
//

#include "Reader.h"

Reader::Reader(std::string path) {
    filePath = std::move(path);
    if (!file.is_open()) {
        file = std::ifstream(filePath);
    }
}

char Reader::getNextChar() {
    char cur = END_CHAR;
    if (!file.is_open()) {
        std::cout << "Error opening file";
        return cur;
    }
    if (!file.eof()) {
        // 不跳过空格
        file.get(cur);
    }
    return cur;
}

Reader::~Reader() {
    if (file.is_open()) {
        file.close();
    }
}

void Reader::setBack() {
    long long int m = file.tellg();
    file.seekg(m -1);
}
