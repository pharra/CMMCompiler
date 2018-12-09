//
// Created by chasi on 2018/12/3.
//

#ifndef CMMCOMPILER_QUATERNARY_H
#define CMMCOMPILER_QUATERNARY_H

#include <string>

#include "Utils/Common.h"

class Quaternary {
public:
    static const std::string JMP;
    static const std::string READ;
    static const std::string WRITE;
    static const std::string _IN;
    static const std::string _OUT;
    static const std::string INT;
    static const std::string REAL;
    static const std::string CHAR;
    static const std::string ARRAY;
    static const std::string ASSIGN;
    static const std::string PLUS;
    static const std::string MINUS;
    static const std::string MUL;
    static const std::string DIV;
    static const std::string GT;
    static const std::string LT;
    static const std::string GET;
    static const std::string LET;
    static const std::string EQ;
    static const std::string NEQ;
    static const std::string CALL;
    static const std::string CALLFH;

    Quaternary(const std::string &op, const std::string &arg1, const std::string &arg2 = nullptr,
               const std::string &result = nullptr, _dataSize line);

    const std::string &getOp() const;

    void setOp(const std::string &op);

    const std::string &getArg1() const;

    void setArg1(const std::string &arg1);

    const std::string &getArg2() const;

    void setArg2(const std::string &arg2);

    const std::string &getResult() const;

    void setResult(const std::string &result);

    void setResult(unsigned long long int i);

private:
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
    _dataSize line;
};


#endif //CMMCOMPILER_QUATERNARY_H
