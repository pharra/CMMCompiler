//
// Created by chasi on 2018/12/9.
//

#include "VarSymbol.h"

VarSymbol::VarSymbol() = default;

VarSymbol::~VarSymbol() = default;

VarSymbol::VarSymbol(const std::string &name, SymbolType type, _dataSize line, _dataSize dataSize, _dataSize _length)
        : AbstractSymbol(name, type, line) {
    this->dataSize = dataSize;
    this->_length = _length;
}

_dataSize VarSymbol::getDataSize() const {
    return dataSize;
}

void VarSymbol::setDataSize(_dataSize dataSize) {
    VarSymbol::dataSize = dataSize;
}

_dataSize VarSymbol::length() const {
    return _length;
}

void VarSymbol::setLength(_dataSize _length) {
    VarSymbol::_length = _length;
}
