//
// Created by WF on 2018/9/7.
//

#ifndef CMMCOMPILER_LEXER_H
#define CMMCOMPILER_LEXER_H

#include <string>
#include "Utils/Reader.h"
#include "Structure/Token.h"
#include <set>
#include "Regex.h"

class Lexer {
private:
    Reader *reader;
    int column = 1;
    int line = 1;
    std::set<std::string> keywordSet;
    char currentChar;
    std::string specialChar = "+-*/(){}[]!=<>,;";
    Regex *regex;


public:
    explicit Lexer(std::string filePath);

    Token getNext();

    ~Lexer();
};


#endif //CMMCOMPILER_LEXER_H
