//
// Created by chasi on 2018/12/4.
//

#ifndef CMMCOMPILER_VALUE_H
#define CMMCOMPILER_VALUE_H

#include "Utils/Common.h"

#include <string>

class Value {
public:

    static int addrUnitSize;
    static int callfrom;

    Value();

    Value(const std::string &v, ValueType t);

    const std::string &getValue() const;

    void setValue(const std::string &value);

    ValueType getType() const;

    void setType(ValueType type);

    unsigned long long int getSize() const;

    void setSize(unsigned long long int size);

    std::string typeToStr();

    friend int toInt(Value value);

    friend double toDouble(Value value);

    friend std::string nequal(const Value &l_value, const Value &r_value);

    /*
     * 重载运算符
     */
    friend std::string operator+(const Value &l_value, const Value &r_value);

    friend std::string operator-(const Value &l_value, const Value &r_value);

    friend std::string operator*(const Value &l_value, const Value &r_value);

    friend std::string operator/(const Value &l_value, const Value &r_value);

    friend std::string operator>(const Value &l_value, const Value &r_value);

    friend std::string operator>=(const Value &l_value, const Value &r_value);

    friend std::string operator<(const Value &l_value, const Value &r_value);

    friend std::string operator<=(const Value &l_value, const Value &r_value);

    friend std::string operator==(const Value &l_value, const Value &r_value);

private:
    std::string value;
    ValueType type;
    unsigned long long int size;
};


#endif //CMMCOMPILER_VALUE_H
