//
// Created by chasi on 2018/11/26.
//

#include "Symbol.h"

Symbol::Symbol() = default;

int Symbol::getLevel() const {
    return level;
}

void Symbol::setLevel(int level) {
    Symbol::level = level;
}

Symbol::Symbol(const std::string &value, Symbol::SymbolType type, int size) : value(value), type(type),
                                                                              size(size) {}

Symbol::SymbolType Symbol::getType() const {
    return type;
}

void Symbol::setType(Symbol::SymbolType type) {
    Symbol::type = type;
}

int Symbol::getSize() const {
    return size;
}

void Symbol::setSize(int size) {
    Symbol::size = size;
}

const std::string &Symbol::getValue() const {
    return value;
}

void Symbol::setValue(const std::string &value) {
    Symbol::value = value;
}
