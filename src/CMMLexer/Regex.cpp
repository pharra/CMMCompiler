//
// Created by WF on 2018/9/7.
//


#include "Regex.h"

std::regex * Regex::regexIdentifier = nullptr;

std::regex * Regex::regexNum = nullptr;

bool Regex::isIdentifier(std::string value) {
    if (regexIdentifier == nullptr) {
        // 标识符的正则匹配 R"()"是c++中的原生字符串 \x7f-\xff是ansi里的某些字符
        regexIdentifier = new std::regex(R"(^[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*$)");
    }
    return regex_match(value, *regexIdentifier);
}

bool Regex::isNum(std::string value) {
    if (regexNum == nullptr) {
        // 标识符的正则匹配 R"()"是c++中的原生字符串 \x7f-\xff是ansi里的某些字符
        regexNum = new std::regex(R"(^(\-|\+)?(0)?[1-9]*(\.[\d]+)?$)");
    }
    return regex_match(value, *regexNum);
}

