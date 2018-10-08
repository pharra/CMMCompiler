//
// Created by WF on 2018/9/6.
//

#ifndef CMMCOMPILER_READER_H
#define CMMCOMPILER_READER_H

#include <string>
#include <fstream>
#include <iostream>

#define END_CHAR (char)(-1)

class Reader {
private:
    std::string filePath;
    std::string fileContent;
    int cur = 0;
public:
    explicit Reader(std::string path);

    char getNextChar();

    void setBack();

};


#endif //CMMCOMPILER_READER_H
