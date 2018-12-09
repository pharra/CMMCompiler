//
// Created by chasi on 2018/12/9.
//

#ifndef CMMCOMPILER_FUNCTIONSYMBOL_H
#define CMMCOMPILER_FUNCTIONSYMBOL_H

#include <map>
#include <string>

#include "AbstractSymbol.h"
#include "VarSymbol.h"

class FunctionSymbol : public AbstractSymbol {
public:
    FunctionSymbol(const std::string &value, SymbolType type, _dataSize line);

    ~FunctionSymbol() override;

    SymbolType getReturnType() const;

    void setReturnType(SymbolType returnType);

    bool insertParam(VarSymbol *symbol);

private:
    std::map<std::string, VarSymbol *> paramTable;
};


#endif //CMMCOMPILER_FUNCTIONSYMBOL_H
