//
// Created by chasi on 2018/11/26.
//

#ifndef CMMCOMPILER_SYMBOLTABLE_H
#define CMMCOMPILER_SYMBOLTABLE_H

#include <map>
#include <string>
#include <list>

#include "Symbol.h"
#include "Structure/TreeNode.h"

class SymbolTable {
public:
    SymbolTable();

    void insertTable();

    void destroyTable();

    bool insertVar(Symbol * symbol);

    bool find(std::string key);

    std::string getNewTempSymbolName();

    void clear();

    ~SymbolTable();


private:
    int tmpPostFix = 0;

    struct Table {
        std::map<std::string, Symbol *> table;
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

    std::map<std::string, Symbol *> tmpTable;
};


#endif //CMMCOMPILER_SYMBOLTABLE_H
