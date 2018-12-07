//
// Created by chasi on 2018/12/4.
//

#ifndef CMMCOMPILER_VALUE_H
#define CMMCOMPILER_VALUE_H

#include <string>

class Value {
public:
    enum ValueType {
        INT_VALUE,
        REAL_VALUE,
        STR_VALUE,
        ADDR,
    };

    Value();

    Value(const std::string &v, ValueType t);

private:
    std::string value;
    ValueType type;
    unsigned long long int size;
};


#endif //CMMCOMPILER_VALUE_H
