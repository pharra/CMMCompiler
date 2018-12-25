//
// Created by chasi on 2018/11/28.
//

#include "ExecutorException.h"

ExecutorException::ExecutorException(int lineNo, const std::string &errorStr)
        : m_lineNo(lineNo), m_errorStr(errorStr) {
}

std::string ExecutorException::message() {
    std::stringstream ss;
    ss << "line " << m_lineNo << ": " << m_errorStr;
    return ss.str();
}
