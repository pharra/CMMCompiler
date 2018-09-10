//
// Created by WF on 2018/9/7.
//

#include "Lexer.h"

Lexer::Lexer(std::string filePath) {
    reader = new Reader(filePath);
}

Token *Lexer::getNext() {
    Token * token;
    return token;
}

Lexer::~Lexer() {
    delete reader;
    reader = nullptr;
}
