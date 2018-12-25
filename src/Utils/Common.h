//
// Created by chasi on 2018/12/9.
//

#ifndef CMMCOMPILER_COMMON_H
#define CMMCOMPILER_COMMON_H

#include <vector>
#include <string>
#include <map>
#include <sstream>

typedef unsigned long long int _dataSize;
typedef unsigned long long int _dataLength;
typedef long long int _LL;
typedef unsigned long long int PointAddress;

std::string ctos(char ch);

enum class ValueType : int {
    INT_VALUE = 1,
    CHAR_VALUE = 0,
    REAL_VALUE = 3,
    ADDR = -1,
};

enum class SymbolType : int {
    FUNCTION = -1,
    CLASS = -2,
    TEMP = -3,
    REG = -4,
    INT = 0x1,
    REAL = 0x3,
    CHAR = 0x0,
    ARRAY_INT = 0x11,
    ARRAY_REAL = 0x13,
    ARRAY_CHAR = 0x10,
};
extern const void *DATA_ADDR;   //数据段起始地址(1M)
extern const void *GLOBAL_ADDR; //全局区起始地址(1M)
extern const void *CONST_ADDR;   //常量区起始地址(1M)
extern const void *CODE_ADDR;   //常量区起始地址(1M)

extern std::vector<_dataSize> globalVarVec;

#endif //CMMCOMPILER_COMMON_H
