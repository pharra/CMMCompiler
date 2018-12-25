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
    auto result = table->table.insert(std::map<std::string, VarSymbol *>::value_type(symbol->getName(),
                                                                                     symbol));
    if (result.second) {
        curVarSymbol = symbol;
    }
    return result.second;
}

bool SymbolTable::insertFunc(FunctionSymbol *symbol) {
    auto result = funcTable.insert(std::map<std::string, FunctionSymbol *>::value_type(symbol->getName(), symbol));
    if (result.second) {
        curFuncSymbol = symbol;
    }
    return result.second;
}

VarSymbol *SymbolTable::getVarSymbol(std::string name) {
    Table *tmp = table;
    while (tmp != nullptr) {
        auto i = tmp->table.find(name);
        if (i != tmp->table.end()) {
            return i->second;
        }
        tmp = tmp->parent;
    }
    if (name.find("#T") != -1) {
        return new VarSymbol(name, SymbolType::TEMP, 0, 0);
    }
    return nullptr;
}

void SymbolTable::clear() {
    delete table;
    table = new Table;
}

std::string SymbolTable::getNewTempSymbol() {
    std::string tmpName = "#T" + std::to_string(tmpPostFix++);
    auto *tmp = new VarSymbol(tmpName, SymbolType::TEMP, 0, 0);
    tmpTable.insert(std::map<std::string, VarSymbol *>::value_type(tmpName, tmp));
    return tmpName;
}

bool SymbolTable::isRoot() {
    return table == root;
}

FunctionSymbol *SymbolTable::getCurFuncSymbol() {
    return curFuncSymbol;
}

FunctionSymbol *SymbolTable::getFuncSymbol(std::string name) {
    auto i = funcTable.find(name);
    if (i != funcTable.end()) {
        return i->second;
    }
    return nullptr;
}

VarSymbol *SymbolTable::getCurVarSymbol() {
    return curVarSymbol;
}

bool SymbolTable::insertClass(ClassSymbol *symbol) {
    auto result = classTable.insert(std::map<std::string, ClassSymbol *>::value_type(symbol->getName(), symbol));
    return result.second;
}

ClassSymbol *SymbolTable::getClassSymbol(std::string name) {
    auto i = classTable.find(name);
    if (i != classTable.end()) {
        return i->second;
    }
    return nullptr;
}

