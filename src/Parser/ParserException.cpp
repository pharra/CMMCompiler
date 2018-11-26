#include <utility>

//
// Created by chasi on 2018/11/13.
//

#include "ParserException.h"

ParserException::ParserException(std::string error, int line, ErrorType errorType) {
    this->line = line;
    this->errorMsg = std::move(error);
    this->errorType = errorType;
}

std::string ParserException::message() {
    std::stringstream tokenStream;
    tokenStream << "line " << line << "  message:" + errorMsg;
    return tokenStream.str();
}

ParserException::ErrorType ParserException::getErrorType() const {
    return errorType;
}
