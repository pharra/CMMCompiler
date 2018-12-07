//
// Created by chasi on 2018/11/26.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable() {
    table = new Table;
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

bool SymbolTable::insertVar(Symbol * symbol) {
    if (table->table.find(symbol->getValue()) != table->table.end()) {
        return false;
    }
    table->table.insert(std::map<std::string, Symbol *>::value_type(symbol->getValue(),
                                                                    symbol));
    return true;
}

bool SymbolTable::find(std::string key) {
    Table *tmp = table;
    while (tmp != nullptr) {
        if (table->table.find(key) != table->table.end()) {
            return true;
        }
        tmp = tmp->parent;
    }
    return false;
}

void SymbolTable::clear() {
    delete table;
    table = new Table;
}

SymbolTable::~SymbolTable() {
    delete table;
    table = nullptr;

}

std::string SymbolTable::getNewTempSymbolName() {
    std::string tmpName = "temp" + std::to_string(tmpPostFix++);
    auto *tmp = new Symbol();
    tmpTable.insert(std::map<std::string, Symbol *>::value_type(tmpName, tmp));
    return tmpName;
}

