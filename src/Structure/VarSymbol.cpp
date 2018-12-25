//
// Created by chasi on 2018/12/9.
//

#include "VarSymbol.h"

VarSymbol::VarSymbol() = default;

VarSymbol::~VarSymbol() = default;

VarSymbol::VarSymbol(const std::string &name, SymbolType type, _dataSize line, int level, _dataSize _length)
        : AbstractSymbol(name, type, line, level) {
    switch (type) {
        case SymbolType::INT:
        case SymbolType::ARRAY_INT:
            dataSize = 4;
            break;
        case SymbolType::REAL:
        case SymbolType::ARRAY_REAL:
            dataSize = 8;
            break;
        case SymbolType::CHAR:
        case SymbolType::ARRAY_CHAR:
            dataSize = 1;
            break;
        default:
            break;
    }
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

_dataSize VarSymbol::getValueIndex() const {
    return valueIndex;
}

void VarSymbol::setValueIndex(_dataSize valueIndex) {
    VarSymbol::valueIndex = valueIndex;
}

VarSymbol::VarSymbol(_dataSize valueIndex, SymbolType type) : AbstractSymbol("", type, 0, -1), valueIndex(valueIndex) {
}
