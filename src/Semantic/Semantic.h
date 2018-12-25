//
// Created by chasi on 2018/11/28.
//

#ifndef CMMCOMPILER_SEMANTIC_H
#define CMMCOMPILER_SEMANTIC_H

#include <string>
#include <vector>
#include <stack>

#include "Structure/SymbolTable.h"
#include "Structure/Quaternary.h"
#include "SemanticException.h"
#include "Structure/Value.h"
#include "Utils/Common.h"
#include "Structure/Object.h"

class Semantic {
public:
    long long int programEntry;

    Semantic();

    std::vector<Quaternary> generateCode(const std::vector<TreeNode *> &treeNodeVec, std::string &errorStr);

private:
    /*enum {
        ALL, VALUE, ADDR, CHAR
    };*/
    std::stack<ValueType> typeStack;     //存放结果的类型
    std::vector<_dataSize> returnJmpIndexVec;   //存放return语句的jump地址
    int level, curFunLevel;
    _dataSize curStmtLineNo;
    std::vector<Quaternary> quaternaryVector;
    SymbolTable symbolTable;

    void interpret(const TreeNode *treeNode);

    void interpretIfStmt(const TreeNode *treeNode);

    void interpretWhileStmt(const TreeNode *treeNode);

    void interpretForStmt(const TreeNode *treeNode);

    void interpretBlockStmt(const TreeNode *treeNode);

    void interpretClassStmt(const TreeNode *treeNode);

    void interpretReadStmt(const TreeNode *treeNode);

    void interpretWriteStmt(const TreeNode *treeNode);

    void interpretDecalreStmt(const TreeNode *treeNode, bool isClass = false, Object* object = nullptr);

    void interpretAssignStmt(const TreeNode *treeNode);

    void interpretFunction(const TreeNode *treeNode, bool isClass = false, Object* object = nullptr);

    void interpretReturnStmt(const TreeNode *treeNode);

    std::string interpretFunctionCall(const TreeNode *treeNode);

    std::string interpretExp(const TreeNode *treeNode);

    std::string interpretLogicExp(const TreeNode *treeNode);

    std::string interpretAddtiveExp(const TreeNode *treeNode);

    std::string interpretTermExp(const TreeNode *treeNode);

    std::string interpretVar(const TreeNode *treeNode);

    std::string interpretScript(const TreeNode *treeNode);

    //解释declare语句辅助方法
    void checkDeclareIsRight(Token::TokenTag declarType);

    //检查赋值类型是否匹配
    void checkAssignIsRight(ValueType varType);

    //解释addtiveExp辅助方法
    std::string transformAddOp(int op);

    std::string
    interpretAddtiveExpUtil(const std::string &addOp, const std::string &leftNodeStr, const std::string &rightNodeStr,
                            ValueType leftType, ValueType rightType);

    void checkAddtiveExpIsRight(Token::TokenTag opType);

    //解释term辅助方法
    std::string transformMulOp(int op);

    std::string
    interpretTermUtil(const std::string &mulOp, const std::string &leftNodeStr, const std::string &rightNodeStr,
                      ValueType leftType, ValueType rightType);

    void checkTermIsRight();

    //判断字符串是否是常量
    bool isConstant(const std::string &str, ValueType &type);

    //判断数组下标是否合法
    bool isSubscriptRight(const std::string &script);

    //判断赋值是否合法
    // bool isAssignRight(int varType, const std::string &valueStr);

    //将function类型转为Value类型
    ValueType transformFunToValue(SymbolType funType);
};


#endif //CMMCOMPILER_SEMANTIC_H
