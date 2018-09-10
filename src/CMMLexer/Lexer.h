//
// Created by WF on 2018/9/7.
//

#ifndef CMMCOMPILER_LEXER_H
#define CMMCOMPILER_LEXER_H

#include <string>
#include "Utils/Reader.h"
#include "Structure/Token.h"

class Lexer {
private:
    Reader *reader;
public:
    explicit Lexer(std::string filePath);

    Token *getNext();

    ~Lexer();
};


#endif //CMMCOMPILER_LEXER_H
