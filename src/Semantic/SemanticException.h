//
// Created by chasi on 2018/12/3.
//

#ifndef CMMCOMPILER_SEMANTICEXCEPTION_H
#define CMMCOMPILER_SEMANTICEXCEPTION_H

#include <string>
#include <sstream>

#include "Utils/Common.h"

class SemanticException {
public:
    SemanticException(_dataSize l, const std::string &err);

    std::string message();

private:
    _dataSize line;
    std::string error;
};


#endif //CMMCOMPILER_SEMANTICEXCEPTION_H
