//
// Created by chasi on 2018/11/28.
//

#ifndef CMMCOMPILER_EXECUTOREXCEPTION_H
#define CMMCOMPILER_EXECUTOREXCEPTION_H

#include <string>
#include <sstream>

class ExecutorException
{
public:
    ExecutorException(int lineNo, const std::string & errorStr);
    std::string message();

private:
    int m_lineNo;
    std::string m_errorStr;
};

#endif // CMMCOMPILER_EXECUTOREXCEPTION_H
