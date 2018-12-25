//
// Created by chasi on 2018/12/9.
//

#include "FunctionSymbol.h"

FunctionSymbol::FunctionSymbol(const std::string &value, SymbolType type, _dataSize line) : AbstractSymbol(value, type,
                                                                                                           line, 0) {

}

SymbolType FunctionSymbol::getReturnType() const {
    return getType();
}

void FunctionSymbol::setReturnType(SymbolType returnType) {
    setType(returnType);
}

void FunctionSymbol::insertParam(SymbolType symbolType) {
    params.push_back(symbolType);
}

FunctionSymbol::~FunctionSymbol() = default;

std::vector<SymbolType> &FunctionSymbol::getParams() {
    return params;
}
