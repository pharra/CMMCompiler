//
// Created by WF on 2018/9/7.
//


#include "Regex.h"

bool Regex::isIdentifier(std::string value) {
    return regex_match(value, regexIdentifier);
}

bool Regex::isNum(std::string value) {
    return regex_match(value, regexNum);
}

