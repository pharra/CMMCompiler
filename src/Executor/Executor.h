//
// Created by chasi on 2018/11/28.
//

#ifndef CMMCOMPILER_EXECUTOR_H
#define CMMCOMPILER_EXECUTOR_H

#include <map>
#include <stack>
#include <sstream>
#include <vector>
#include <iostream>

#include "Utils/Common.h"
#include "Structure/Quaternary.h"
#include "Structure/SymbolTable.h"
#include "Structure/Value.h"
#include "ExecutorException.h"

class Executor {
public:
    Executor(long long int programEntry, std::vector<Quaternary> &quaternaryVector);

    void execute();

private:

    void executeStmt();

    void executeJmp(Quaternary &quaternary);

    void executeRead(Quaternary &quaternary);

    void executeWrite(Quaternary &quaternary);

    void executeIn(Quaternary &quaternary);

    void executeOut(Quaternary &quaternary);

    void executeInt(Quaternary &quaternary);

    void executeReal(Quaternary &quaternary);

    void executeChar(Quaternary &quaternary);

    void executeArray(Quaternary &quaternary);

    void executeAssign(Quaternary &quaternary);

    void executePlus(Quaternary &quaternary);

    void executeMinus(Quaternary &quaternary);

    void executeMul(Quaternary &quaternary);

    void executeDiv(Quaternary &quaternary);

    void executeGt(Quaternary &quaternary);

    void executeLt(Quaternary &quaternary);

    void executeGet(Quaternary &quaternary);

    void executeLet(Quaternary &quaternary);

    void executeEq(Quaternary &quaternary);

    void executeNeq(Quaternary &quaternary);

    void executeCall(Quaternary &quaternary);

    void executeCallFH(Quaternary &quaternary);

    void executeRet(Quaternary &quaternary);


private:
    long long int programEntry;
    _dataSize curExecuteStmtLineNo;
    std::vector<Quaternary> quaternaryVector; // 四元式代码段
    long long int nextQeVecIndex;        //当前代码段索引
    int ebp;                  //栈底
    int esp;                  //栈顶
    std::stack<PointAddress> previousEbp;          //上一层函数的栈底
    std::stack<PointAddress> previousEsp;          //上一层的栈顶
    std::vector<Value> valueVec;      //数据段
    std::map<PointAddress, PointAddress> addrMap;       //数据段索引,地址
    PointAddress curDataAddr;          //当前数据段地址
    SymbolTable symbolTable;          //符号表
    std::map<std::string, Value> tempMap;     //存放临时变量
    std::vector<Value> constVec;      //常量区
    std::map<PointAddress, PointAddress> constMap;      //常量区索引，地址
    PointAddress curConstAddr;         //当前常量区地址
    bool isOver;                //程序是否执行完毕
    std::vector<Value> globalVec;     //全局区
    std::map<PointAddress, PointAddress> globalMap;     //全局区索引，地址
    PointAddress curGlobalAddr;                //当前全局区地址
    std::stringstream inputStream;

private:
    int level;

    int getStringType(const std::string &str);     //根据四元式的内容获取其类型
    Value readValue(const std::string &str);

    void assignProcess(const VarSymbol &symbol, int index, int dereference, const std::string &str);   //赋值操作
    void assignUtil(_dataSize valueIndex, _dataSize index, const std::string valueStr, ValueType valueType,
                    bool isGlobal);      //赋值辅助操作
    void declareProcess(const Quaternary &qExp, const std::string &type);      //声明操作
    void checkAddressIsRight(int pointType, const std::string &addrValue);    //判断地址类型是否正确
    int getValueIndex(const std::string &addrValue, int &addrRange, int &offset);      //根据地址锁定其位置
    int getScript(const std::string varStr);        //如果是数组，则获取数组下标，否则返回-1
    int getRegNum(const std::string str);           //获取"@ebp..."的值
    std::string addrFromStr(const std::string &addr);  //根据字符串获取地址
    std::string addrFromInt(int addr);      //根据int获取地址
    void setAddrUnitSize(int symbolType);       //设置addrUntiSize的值
    void setAddrUnitSize(const std::string &addrStr);

    void inputToStream(std::string value);
};

#endif // CMMCOMPILER_EXECUTOR_H
