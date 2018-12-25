//
// Created by chasi on 2018/12/3.
//

#include "SemanticException.h"

SemanticException::SemanticException(_dataSize l, const std::string &err)
        : line(l), error(err) {}

std::string SemanticException::message() {
    std::stringstream stringSteam;
    stringSteam << "line " << line << ": " << error;
    return stringSteam.str();
}