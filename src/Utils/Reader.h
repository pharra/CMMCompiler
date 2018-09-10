//
// Created by WF on 2018/9/6.
//

#ifndef CMMCOMPILER_READER_H
#define CMMCOMPILER_READER_H

#include <string>
#include <fstream>
#include <iostream>

class Reader {
private:
    std::string filePath;
    std::ifstream file;
public:
    explicit Reader(std::string path);

    std::string getNextChar();

    ~Reader();
};


#endif //CMMCOMPILER_READER_H
