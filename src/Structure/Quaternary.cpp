//
// Created by chasi on 2018/12/3.
//

#include "Quaternary.h"

const std::string Quaternary::JMP = "jmp";
const std::string Quaternary::READ = "read";
const std::string Quaternary::WRITE = "write";
const std::string Quaternary::IN = "in";
const std::string Quaternary::OUT = "out";
const std::string Quaternary::INT = "int";
const std::string Quaternary::REAL = "real";
const std::string Quaternary::CHAR = "char";
const std::string Quaternary::INT_POINT = "int*";
const std::string Quaternary::REAL_POINT = "real*";
const std::string Quaternary::CHAR_POINT = "char*";
const std::string Quaternary::ASSIGN = "assign";
const std::string Quaternary::PLUS = "+";
const std::string Quaternary::MINUS = "-";
const std::string Quaternary::MUL = "*";
const std::string Quaternary::DIV = "/";
const std::string Quaternary::GT = ">";
const std::string Quaternary::LT = "<";
const std::string Quaternary::GET = ">=";
const std::string Quaternary::LET = "<=";
const std::string Quaternary::EQ = "==";
const std::string Quaternary::NEQ = "<>";
const std::string Quaternary::ADDR = "&";
const std::string Quaternary::CALL = "call";
const std::string Quaternary::CALLFH = "callfh";
const std::string Quaternary::VOID = "void";

const std::string &Quaternary::getOp() const {
    return op;
}

void Quaternary::setOp(const std::string &op) {
    Quaternary::op = op;
}

const std::string &Quaternary::getArg1() const {
    return arg1;
}

void Quaternary::setArg1(const std::string &arg1) {
    Quaternary::arg1 = arg1;
}

const std::string &Quaternary::getArg2() const {
    return arg2;
}

void Quaternary::setArg2(const std::string &arg2) {
    Quaternary::arg2 = arg2;
}

const std::string &Quaternary::getResult() const {
    return result;
}

void Quaternary::setResult(const std::string &result) {
    Quaternary::result = result;
}

Quaternary::Quaternary(const std::string &op, const std::string &arg1, const std::string &arg2,
                       const std::string &result) : op(op), arg1(arg1), arg2(arg2), result(result) {}

void Quaternary::setResult(unsigned long long int i) {
    result = std::to_string(i);
}
