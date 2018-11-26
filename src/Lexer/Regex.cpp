//
// Created by WF on 2018/9/7.
//


#include "Regex.h"

/**
 * 判断是否是合法的标识符
 * @param value 字符串
 * @return bool
 */
bool Regex::isIdentifier(const std::string value) {
    return std::regex_match(value, regexIdentifier);
}

/**
 * 判断是否是合法的数字
 * @param value 字符串
 * @return bool
 */
bool Regex::isNum(const std::string value) {
    return std::regex_match(value, regexNum);
}

