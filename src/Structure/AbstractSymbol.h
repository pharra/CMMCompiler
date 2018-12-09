//
// Created by chasi on 2018/11/26.
//

#ifndef CMMCOMPILER_RECORD_H
#define CMMCOMPILER_RECORD_H

#include "Utils/Common.h"
#include "Structure/Token.h"

class AbstractSymbol {
public:

    AbstractSymbol();

    AbstractSymbol(const std::string &name, SymbolType type, _dataSize line);

    virtual ~AbstractSymbol() = 0;

    const std::string &getName() const;

    void setName(const std::string &name);

    SymbolType getType() const;

    void setType(SymbolType type);

    _dataSize getLine() const;

    void setLine(_dataSize line);

private:
    std::string name;

    SymbolType type;

    _dataSize line;
};


#endif //CMMCOMPILER_RECORD_H
