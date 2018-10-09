//
// Created by WF on 2018/9/7.
//


#include "Regex.h"

bool Regex::isIdentifier(const std::string value) {
    return std::regex_match(value, regexIdentifier);
}

bool Regex::isNum(const std::string value) {
    return std::regex_match(value, regexNum);
}

