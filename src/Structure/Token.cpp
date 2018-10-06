#include <utility>

//
// Created by WF on 2018/9/6.
//

#include "Token.h"

Token::Token(TokenTag t, std::string v, int l, int c) {
    tag = t;
    value = std::move(v);
    line = l;
    column = c;
}

TokenTag Token::getTag() const {
    return tag;
}

const std::string &Token::getValue() const {
    return value;
}

int Token::getLine() const {
    return line;
}

int Token::getColumn() const {
    return column;
}
