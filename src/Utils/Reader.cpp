//
// Created by WF on 2018/9/6.
//

#include "Reader.h"

Reader::Reader(std::string path) {
    std::ifstream file;
    filePath = std::move(path);
    file = std::ifstream(filePath);
    if (!file.is_open()) {
        std::cout << "Error opening file";
    }
    fileContent = std::string((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    file.close();
}

char Reader::getNextChar() {
    if(cur > fileContent.size()){
        return END_CHAR;
    }
    return fileContent[cur++];
}

void Reader::setBack() {
    cur--;
}
