//
// Created by chasi on 2018/12/4.
//

#include "Value.h"

int Value::addrUnitSize = 1;
int Value::callfrom = -10;

Value::Value() = default;

Value::Value(const std::string &v, ValueType t) : value(v), type(t) {
    bool isChar = false;
    if (v.at(0) == '\'')
        isChar = true;
    switch (type) {
        case ValueType::INT_VALUE:
            size = 4;
            if (isChar)
                value = v.at(1);
            else
                value = std::to_string(atoi(v.c_str()));
            break;
        case ValueType::REAL_VALUE:
            size = 8;
            if (isChar)
                value = v.at(1);
            else
                value = std::to_string(atof(v.c_str()));
            break;
        case ValueType::CHAR_VALUE:
            size = 1;
            if (isChar)
                value = v;
            else
                value = "'" + std::to_string(char(atoi(v.c_str()))) + "'";
            break;
        case ValueType::ADDR:
            size = 4;
            value = v;
            break;
        default:
            break;
    }
}


std::string Value::typeToStr() {
    switch (type) {
        case ValueType::INT_VALUE:
            return "int";
        case ValueType::REAL_VALUE:
            return "real";
        case ValueType::CHAR_VALUE:
            return "char";
        case ValueType::ADDR:
            return "address";
        default:
            return "";
    }
}

int toInt(Value value) {
    switch (value.type) {
        case ValueType::CHAR_VALUE:
            return value.value.at(1);
        case ValueType::INT_VALUE:
            return std::stol(value.value);
        case ValueType::ADDR:
            return std::stol(value.value.substr(1, value.value.size() - 1));
        case ValueType::REAL_VALUE:
            throw ("无法将浮点数和地址进行运算");
    }
    return 0;
}

double toDouble(Value value) {
    switch (value.type) {
        case ValueType::CHAR_VALUE:
            return value.value.at(1);
        case ValueType::INT_VALUE:
        case ValueType::REAL_VALUE:
            return std::stol(value.value);
    }
    return 0;
}


std::string nequal(const Value &l_value, const Value &r_value) {
    int result = 0;
    if ((l_value == r_value) == "0")
        result = 1;
    return std::to_string(result);
}

std::string operator+(const Value &l_value, const Value &r_value) {
    if ((l_value.getType() == ValueType::INT_VALUE || l_value.getType() == ValueType::CHAR_VALUE)
        && (r_value.getType() == ValueType::INT_VALUE || r_value.getType() == ValueType::CHAR_VALUE)) {
        int result = toInt(l_value) + toInt(r_value);
        return std::to_string(result);
    } else if (l_value.getType() == ValueType::ADDR) {
        int result = toInt(l_value) + toInt(r_value) * Value::addrUnitSize;
        return "$" + std::to_string(result);
    } else if (r_value.getType() == ValueType::ADDR) {
        int result = toInt(l_value) * Value::addrUnitSize + toInt(r_value);
        return "$" + std::to_string(result);
    } else if (l_value.getType() == ValueType::REAL_VALUE || r_value.getType() == ValueType::REAL_VALUE) {
        double result = toDouble(l_value) + toDouble(r_value);
        return std::to_string(result);
    }
    return "0";
    //throw ExecutorException(curExecuteStmtLineNo, "运算表达式错误");
}

std::string operator-(const Value &l_value, const Value &r_value) {
    if ((l_value.getType() == ValueType::INT_VALUE || l_value.getType() == ValueType::CHAR_VALUE)
        && (r_value.getType() == ValueType::INT_VALUE || r_value.getType() == ValueType::CHAR_VALUE)) {
        int result = toInt(l_value) - toInt(r_value);
        return std::to_string(result);
    } else if (l_value.getType() == ValueType::ADDR) {
        int result = toInt(l_value) - toInt(r_value) * Value::addrUnitSize;
        return "$" + std::to_string(result);
    } else if (l_value.getType() == ValueType::REAL_VALUE || r_value.getType() == ValueType::REAL_VALUE) {
        double result = toDouble(l_value) - toDouble(r_value);
        return std::to_string(result);
    }
    return "0";
    //throw ExecutorException(curExecuteStmtLineNo, "运算表达式错误");
}

std::string operator*(const Value &l_value, const Value &r_value) {
    if ((l_value.getType() == ValueType::INT_VALUE || l_value.getType() == ValueType::CHAR_VALUE)
        && (r_value.getType() == ValueType::INT_VALUE || r_value.getType() == ValueType::CHAR_VALUE)) {
        int result = toInt(l_value) * toInt(r_value);
        return std::to_string(result);
    } else if (l_value.getType() == ValueType::REAL_VALUE || r_value.getType() == ValueType::REAL_VALUE) {
        double result = toDouble(l_value) * toDouble(r_value);
        return std::to_string(result);
    }
    return "0";
    //throw ExecutorException(curExecuteStmtLineNo, "运算表达式错误");
}

std::string operator/(const Value &l_value, const Value &r_value) {
    if ((l_value.getType() == ValueType::INT_VALUE || l_value.getType() == ValueType::CHAR_VALUE)
        && (r_value.getType() == ValueType::INT_VALUE || r_value.getType() == ValueType::CHAR_VALUE)) {
        if (toInt(r_value) == 0) {
            if (Value::callfrom == -10) {
                throw ("除数不能为0");
            } else {
                int tempLineNo = Value::callfrom;
                Value::callfrom = -10;
                throw ("除数不能为0");
            }
        }
        int result = toInt(l_value) / toInt(r_value);
        return std::to_string(result);
    } else if (l_value.getType() == ValueType::REAL_VALUE || r_value.getType() == ValueType::REAL_VALUE) {
        if (toDouble(r_value) == 0) {
            if (Value::callfrom == -10) {
                throw ("除数不能为0");
            } else {
                int tempLineNo = Value::callfrom;
                Value::callfrom = -10;
                throw ("除数不能为0");
            }
        }
        double result = toDouble(l_value) / toDouble(r_value);
        return std::to_string(result);
    }
    return "0";
    //throw ExecutorException(curExecuteStmtLineNo, "运算表达式错误");
}

std::string operator>(const Value &l_value, const Value &r_value) {
    int result = 0;
    if (l_value.getType() == ValueType::ADDR && r_value.getType() == ValueType::ADDR)
        result = (toInt(l_value) > toInt(r_value));
//    else if (l_value.getType() == Value::STR_VALUE && r_value.getType() == Value::STR_VALUE)
//        result = (strcmp(l_value.v(), r_value.v()) > 0);
    else
        result = (std::stod((l_value - r_value)) > 0);
    return std::to_string(result);
}

std::string operator>=(const Value &l_value, const Value &r_value) {
    int result = 0;
    if (l_value.getType() == ValueType::ADDR && r_value.getType() == ValueType::ADDR)
        result = (toInt(l_value) >= toInt(r_value));
    else
        result = (std::stod((l_value - r_value)) >= 0);
    return std::to_string(result);
}

std::string operator<(const Value &l_value, const Value &r_value) {
    int result = 0;
    if (r_value > l_value != "0")
        result = 1;
    return std::to_string(result);
}

std::string operator<=(const Value &l_value, const Value &r_value) {
    int result = 0;
    if (r_value >= l_value != "0")
        result = 1;
    return std::to_string(result);
}

std::string operator==(const Value &l_value, const Value &r_value) {
    int result = 0;
    if (l_value.getType() == ValueType::ADDR && r_value.getType() == ValueType::ADDR)
        result = (toInt(l_value) == toInt(r_value));
        /*else if (l_value.getType() == Value::STR_VALUE && r_value.getType() == Value::STR_VALUE)
            result = (l_value.value == r_value.value);*/
    else
        result = (std::stod((l_value - r_value)) == 0);
    return std::to_string(result);
}

const std::string &Value::getValue() const {
    return value;
}

void Value::setValue(const std::string &value) {
    Value::value = value;
}

ValueType Value::getType() const {
    return type;
}

void Value::setType(ValueType type) {
    Value::type = type;
}

unsigned long long int Value::getSize() const {
    return size;
}

void Value::setSize(unsigned long long int size) {
    Value::size = size;
}
