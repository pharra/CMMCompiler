//
// Created by chasi on 2018/11/26.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable() {
    table = new Table;
    root = table;
}

SymbolTable::~SymbolTable() {
    delete table;
    table = nullptr;
    root = nullptr;
}

void SymbolTable::destroyTable() {
    Table *tmp = table;
    table = table->parent;

    tmp->parent = nullptr;
    delete tmp;
}

void SymbolTable::insertTable() {
    auto *tmp = new Table;
    tmp->parent = table;
    table = tmp;
}

bool SymbolTable::insertVar(VarSymbol *symbol) {
    if (table->table.find(symbol->getName()) != table->table.end()) {
        return false;
    }
    table->table.insert(std::map<std::string, AbstractSymbol *>::value_type(symbol->getName(),
                                                                            symbol));
    return true;
}

AbstractSymbol *SymbolTable::isDeclared(std::string key) {
    Table *tmp = table;
    while (tmp != nullptr) {
        auto i = tmp->table.find(key);
        if (i != tmp->table.end()) {
            return i->second;
        }
        tmp = tmp->parent;
    }
    return nullptr;
}

void SymbolTable::clear() {
    delete table;
    table = new Table;
}

VarSymbol *SymbolTable::getNewTempSymbol() {
    std::string tmpName = "temp" + std::to_string(tmpPostFix++);
    auto *tmp = new VarSymbol();
    tmpTable.insert(std::map<std::string, VarSymbol *>::value_type(tmpName, tmp));
    return tmp;
}

bool SymbolTable::isRoot() {
    return table == root;
}

