//
// Created by WF on 2018/9/6.
//

#include "Token.h"

std::map<Token::TokenTag, std::string> Token::tokenTagMap = {
        {UNDEFINED,     "UNDEFINED"},
        {END,           "END"},
        {IDENTIFIER,    "IDENTIFIER"},
        {NUM,           "NUM"},
        {INT_VALUE,     "INT_VALUE"},
        {REAL_VALUE,    "REAL_VALUE"},
        {CHAR_VALUE,    "CHAR_VALUE"},
        {KEYWORD,       "KEYWORD"},
        {IF,            "IF"},
        {ELSE,          "ELSE"},
        {WHILE,         "WHILE"},
        {FOR,           "FOR"},
        {READ,          "READ"},
        {WRITE,         "WRITE"},
        {INT,           "INT"},
        {REAL,          "REAL"},
        {CHAR,          "CHAR"},
        {BREAK,         "BREAK"},
        {SWITCH,        "SWITCH"},
        {CASE,          "CASE"},
        {RETURN,        "RETURN"},
        {NEW,           "NEW"},
        {CLASS,         "CLASS"},
        {PLUS,          "PLUS"},
        {MINUS,         "MINUS"},
        {MUL,           "MUL"},
        {DIV,           "DIV"},
        {MOD,           "MOD"},
        {ASSIGN,        "ASSIGN"},
        {LES,           "LES"},
        {LES_EQL,       "LES_EQL"},
        {GRT,           "GRT"},
        {GRT_EQL,       "GRT_EQL"},
        {EQL,           "EQL"},
        {NOT_EQL,       "NOT_EQL"},
        {LEFT_BRA,      "LEFT_BRA"},
        {RIGHT_BRA,     "RIGHT_BRA"},
        {LEFT_INDEX,    "LEFT_INDEX"},
        {RIGHT_INDEX,   "RIGHT_INDEX"},
        {LEFT_BOUNDER,  "LEFT_BOUNDER"},
        {RIGHT_BOUNDER, "RIGHT_BOUNDER"},
        {COMMA,         "COMMA"},
        {SEMI,          "SEMI"},
        {SIN_QUE,       "SIN_QUE"},
        {DOU_QUE,       "DOU_QUE"},
        {LINE_NOTE,     "LINE_NOTE"},
        {MUL_NOTE,      "MUL_NOTE"},
};

Token::Token() {
    tag = UNDEFINED;
    line = -1;
    column = -1;
    isError = false;
}

Token::Token(TokenTag t, std::string v, int l, int c, std::string error, bool isErr) {
    tag = t;
    value = std::move(v);
    line = l;
    column = c;
    errorMessage = std::move(error);
    isError = isErr;
}

Token::TokenTag Token::getTag() const {
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

std::string Token::getTagName() const {
    return tokenTagMap.find(tag)->second;
}

const std::string &Token::getErrorMessage() const {
    return errorMessage;
}

bool Token::isIsError() const {
    return isError;
}

Token *Token::clone() {
    Token *newTOken = new Token(tag, value, line, column, errorMessage, isError);
    return newTOken;
}

