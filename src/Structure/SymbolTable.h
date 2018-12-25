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
#include "ClassSymbol.h"

class SymbolTable {
public:
    SymbolTable();

    ~SymbolTable();

    void insertTable();

    void destroyTable();

    bool insertVar(VarSymbol *symbol);

    VarSymbol *getVarSymbol(std::string name);

    bool insertFunc(FunctionSymbol *symbol);

    FunctionSymbol *getFuncSymbol(std::string name);

    bool insertClass(ClassSymbol *symbol);

    ClassSymbol *getClassSymbol(std::string name);

    std::string getNewTempSymbol();

    void clear();

    /**
     * if table level is 0, return true, else false
     * @return bool
     */
    bool isRoot();

    FunctionSymbol *getCurFuncSymbol();

    VarSymbol *getCurVarSymbol();

private:
    int tmpPostFix = 0;

    struct Table {
        std::map<std::string, VarSymbol *> table;
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
    std::map<std::string, ClassSymbol *> classTable;
    FunctionSymbol *curFuncSymbol = nullptr;
    VarSymbol *curVarSymbol = nullptr;
};


#endif //CMMCOMPILER_SYMBOLTABLE_H
