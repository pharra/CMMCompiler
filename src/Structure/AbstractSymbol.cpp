//
// Created by chasi on 2018/11/26.
//

#include "AbstractSymbol.h"

AbstractSymbol::AbstractSymbol() = default;

AbstractSymbol::AbstractSymbol(const std::string &name,
                               SymbolType type, _dataSize line) : name(name),
                                                                  type(type),
                                                                  line(line) {
}

SymbolType AbstractSymbol::getType() const {
    return type;
}

void AbstractSymbol::setType(SymbolType type) {
    AbstractSymbol::type = type;
}

const std::string &AbstractSymbol::getName() const {
    return name;
}

void AbstractSymbol::setName(const std::string &name) {
    AbstractSymbol::name = name;
}

_dataSize AbstractSymbol::getLine() const {
    return line;
}

void AbstractSymbol::setLine(_dataSize line) {
    AbstractSymbol::line = line;
}
