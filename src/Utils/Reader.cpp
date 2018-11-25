//
// Created by WF on 2018/9/6.
//

#include "Reader.h"

/**
 * 初始化reader类
 * @param path 文件路径
 */
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

/**
 * 获取下一个字符
 * @return char
 */
char Reader::getNextChar() {
    if (cur >= fileContent.size()) {
        return END_CHAR;
    }
    return fileContent[cur++];
}

/**
 * 将指针往前移
 */
void Reader::setBack() {
    if (cur == 0) {
        return;;
    }
    cur--;
}
