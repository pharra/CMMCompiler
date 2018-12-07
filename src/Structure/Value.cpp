//
// Created by chasi on 2018/12/4.
//

#include "Value.h"

Value::Value() = default;

Value::Value(const std::string &v, ValueType t) : value(v), type(t) {
    switch (t) {
        case INT_VALUE:
            size = 4;
            break;
        case REAL_VALUE:
            size = 8;
            break;
        case STR_VALUE:
            size = v.length();
            break;
        case ADDR:
            size = 4;
            break;
    }
}
