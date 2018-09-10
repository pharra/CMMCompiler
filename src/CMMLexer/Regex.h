//
// Created by WF on 2018/9/7.
//

#ifndef CMMCOMPILER_REGEX_H
#define CMMCOMPILER_REGEX_H

#include <string>
#include <regex>


class Regex {
private:
    static std::regex *regexNum;
    static std::regex *regexIdentifier;
public:


    static bool isIdentifier(std::string value);

    static bool isNum(std::string value);
};

#endif //CMMCOMPILER_REGEX_H
