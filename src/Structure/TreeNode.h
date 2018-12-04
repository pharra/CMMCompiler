//
// Created by chasi on 2018/11/12.
//

#ifndef CMMCOMPILER_TREENODE_H
#define CMMCOMPILER_TREENODE_H

#include "Token.h"
#include <vector>
#include <iostream>

class TreeNode {
public:
    enum TreeNodeType {
        NUL,            // 空
        CHARACTER,      // { } ( ) 等字符
        IF_STMT,        // if语句
        WHILE_STMT,     // while语句
        FOR_STMT,       // for语句
        READ_STMT,      // read语句
        WRITE_STMT,     // write语句
        DECLARE_STMT,   // 声明语句
        ASSIGN_STMT,    // 赋值语句
        BLOCK_STMT,     // block语句
        RETURN_STMT,    // return语句
        EXP,            // 表达式
        VAR,            // 变量
        FUNDECLARE,     // 函数声明
        FUNCALL,        // 函数调用
        OP,             // 运算符
        FACTOR,         // 因子
        LITERAL,        // 字面值
        ERRORSTEP,      // 跳过错误
        PARAMS_STMT,    // 参数声明，传入
    };

    static std::map<TreeNodeType, std::string> treeNodeTypeMap;

    explicit TreeNode(TreeNodeType type, Token *t = nullptr);

    ~TreeNode();

    TreeNodeType getTreeNodeType() const;

    Token::TokenTag getDataType() const;

    std::string getValue() const;

    int getLine() const;

    void removeCharacter();

    void toString();

    std::string treeNodeTypeName() const;

    std::vector<TreeNode *> getChild() const;

    void push_back(TreeNode *node);

    int getLevel() const;

    void setLevel(int level);

    Token *getToken() const;

private:
    TreeNodeType treeNodeType;
    Token *token;
    std::vector<TreeNode *> child;
    int level = 0;

};


#endif //CMMCOMPILER_TREENODE_H
