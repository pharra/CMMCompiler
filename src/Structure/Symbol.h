//
// Created by chasi on 2018/11/26.
//

#ifndef CMMCOMPILER_RECORD_H
#define CMMCOMPILER_RECORD_H

#include "Structure/Token.h"

class Symbol {
public:
    enum SymbolType {
        TEMP,
        REG,
        INT,
        REAL,
        CHAR,
        ARRAY_INT,
        ARRAY_REAL,
        ARRAY_CHAR,
        POINT_INT,
        POINT_REAL,
        POINT_CHAR,
        ARRAY_POINT_INT,
        ARRAY_POINT_REAL,
        ARRAY_POINT_CHAR
    };

    Symbol();

    Symbol(const std::string &value, SymbolType type, int size);

    int getLevel() const;

    void setLevel(int level);

    const std::string &getValue() const;

    void setValue(const std::string &value);

    SymbolType getType() const;

    void setType(SymbolType type);

    int getSize() const;

    void setSize(int size);

private:
    std::string value;

    SymbolType type;

    int size;

    int level;
};


#endif //CMMCOMPILER_RECORD_H
