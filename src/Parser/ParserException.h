//
// Created by chasi on 2018/11/13.
//

#ifndef CMMCOMPILER_PARSEREXCEPTION_H
#define CMMCOMPILER_PARSEREXCEPTION_H

#include <string>
#include <sstream>
#include "Structure/Token.h"

class ParserException {
public:
    enum ErrorType{
        FILE_END,
        TOKEN_ERROR,
        NO_EXPECTED_TOKEN,
    };
    ParserException(std::string error, int line, ErrorType errorType);

    std::string message();

    ErrorType getErrorType() const;

private:
    int line;
    std::string errorMsg;
    ErrorType errorType;
};


#endif //CMMCOMPILER_PARSEREXCEPTION_H
