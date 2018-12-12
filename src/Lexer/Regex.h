//
// Created by chasi on 2018/9/7.
//

#ifndef CMMCOMPILER_REGEX_H
#define CMMCOMPILER_REGEX_H

#include <string>
#include <regex>


class Regex {
private:
    std::regex regexIdentifier = std::regex("[a-zA-Z_][a-zA-Z0-9_]*");
    std::regex regexNum = std::regex(R"((0)?[1-9]*(\.[\d]+)?)");
public:

    bool isIdentifier(std::string value);

    bool isNum(std::string value);
};

#endif //CMMCOMPILER_REGEX_H
