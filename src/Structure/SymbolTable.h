//
// Created by chasi on 2018/11/26.
//

#ifndef CMMCOMPILER_SYMBOLTABLE_H
#define CMMCOMPILER_SYMBOLTABLE_H

#include <map>
#include <string>
#include <list>

#include "VarSymbol.h"
#include "FunctionSymbol.h"
#include "Structure/TreeNode.h"

class SymbolTable {
public:
    SymbolTable();

    ~SymbolTable();

    void insertTable();

    void destroyTable();

    bool insertVar(VarSymbol *symbol);

    AbstractSymbol *isDeclared(std::string key);

    VarSymbol *getNewTempSymbol();

    void clear();

    /**
     * if table level is 0, return true, else false
     * @return bool
     */
    bool isRoot();

private:
    int tmpPostFix = 0;

    struct Table {
        std::map<std::string, AbstractSymbol *> table;
        Table *parent;

        Table() {
            parent = nullptr;
        }

        ~Table() {
            delete parent;
            parent = nullptr;
        }
    };

    Table *table;
    Table *root;
    std::map<std::string, VarSymbol *> tmpTable;
    std::map<std::string, FunctionSymbol *> funcTable;

};


#endif //CMMCOMPILER_SYMBOLTABLE_H
