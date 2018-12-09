//
// Created by chasi on 2018/12/9.
//

#include "FunctionSymbol.h"

FunctionSymbol::FunctionSymbol(const std::string &value, SymbolType type, _dataSize line) : AbstractSymbol(value, type,
                                                                                                           line) {

}

SymbolType FunctionSymbol::getReturnType() const {
    return getType();
}

void FunctionSymbol::setReturnType(SymbolType returnType) {
    setType(returnType);
}

bool FunctionSymbol::insertParam(VarSymbol *symbol) {
    if (paramTable.find(symbol->getName()) != paramTable.end()) {
        return false;
    }
    paramTable.insert(std::map<std::string, VarSymbol *>::value_type(symbol->getName(),
                                                                     symbol));
    return true;
}

FunctionSymbol::~FunctionSymbol() {
    for (auto &i : paramTable) {
        delete i.second;
        i.second = nullptr;
    }

    paramTable.clear();
}
