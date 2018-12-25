//
// Created by chasi on 2018/11/26.
//

#ifndef CMMCOMPILER_ABSTRACTSYMBOL_H
#define CMMCOMPILER_ABSTRACTSYMBOL_H

#include "Utils/Common.h"
#include "Structure/Token.h"

class AbstractSymbol {
public:

    AbstractSymbol();

    AbstractSymbol(const std::string &name, SymbolType type, _dataSize line, int level);

    virtual ~AbstractSymbol() = default;

    const std::string &getName() const;

    void setName(const std::string &name);

    SymbolType getType() const;

    void setType(SymbolType type);

    _dataSize getLine() const;

    void setLine(_dataSize line);

    int getLevel() const;

    void setLevel(int level);

private:
    std::string name;

    SymbolType type;

    _dataSize line = 0;

    int level = 0;
};


#endif //CMMCOMPILER_ABSTRACTSYMBOL_H
