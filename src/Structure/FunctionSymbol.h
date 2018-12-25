//
// Created by chasi on 2018/12/9.
//

#ifndef CMMCOMPILER_FUNCTIONSYMBOL_H
#define CMMCOMPILER_FUNCTIONSYMBOL_H

#include <vector>
#include <string>

#include "AbstractSymbol.h"
#include "VarSymbol.h"

class FunctionSymbol : public AbstractSymbol {
public:
    FunctionSymbol(const std::string &value, SymbolType type, _dataSize line);

    ~FunctionSymbol() override;

    SymbolType getReturnType() const;

    void setReturnType(SymbolType returnType);

    void insertParam(SymbolType symbolType);

    std::vector<SymbolType>& getParams();

private:
    std::vector<SymbolType> params;
};


#endif //CMMCOMPILER_FUNCTIONSYMBOL_H
