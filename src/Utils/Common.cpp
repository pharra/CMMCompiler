//
// Created by chasi on 2018/12/10.
//

#include <malloc.h>
#include "Common.h"

std::string ctos(char ch)
{
    std::stringstream ss;
    ss << ch;
    return ss.str();
}

std::vector<_dataSize> globalVarVec;

const void *DATA_ADDR = (void *) 0x50000000;    //数据段起始地址(1M)
const void *GLOBAL_ADDR = (void *) 0x30000000;  //全局区起始地址(1M)
const void *CONST_ADDR = (void *) 0x20000000;   //常量区起始地址(1M)
const void *CODE_ADDR = (void *) 0x10000000;   //常量区起始地址(1M)

/*
const void *DATA_ADDR = malloc(1048576);    //数据段起始地址(1M)
const void *GLOBAL_ADDR = malloc(1048576);  //全局区起始地址(1M)
const void *CONST_ADDR = malloc(1048576);   //常量区起始地址(1M)
const void *CODE_ADDR = malloc(1048576);   //常量区起始地址(1M)*/