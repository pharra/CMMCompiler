//
// Created by chasi on 2018/11/12.
//

#ifndef CMMCOMPILER_TREENODE_H
#define CMMCOMPILER_TREENODE_H

#include "Token.h"

enum TreeNodeType {
    NUL,            //空
    IF_STMT,        //if语句
    WHILE_STMT,     //while语句
    FOR_STMT,       //for语句
    READ_STMT,      //read语句
    WRITE_STMT,     //write语句
    DECLARE_STMT,   //声明语句
    ASSIGN_STMT,    //赋值语句
    RETURN_STMT,    //return语句
    EXP,            //表达式
    VAR,            //变量
    FUNDECLARE,     //函数声明
    FUNCALL,        //函数调用
    VARTYPE,        //变量类型
    OP,             //运算符
    FACTOR,         //因子
    LITERAL,        //字面值
    REG             //寄存器
};

class TreeNode {
private:
    TreeNodeType treeNodeType;
    Token *token;
    TreeNode *left;
    TreeNode *middle;
    TreeNode *right;
    TreeNode *next;

public:

    TreeNode(Token *t, TreeNodeType type);

    ~TreeNode();

    TokenTag getDataType() const;

    std::string getValue() const;

    int getLine() const;

    TreeNode *getLeft() const;

    void setLeft(TreeNode *left);

    TreeNode *getMiddle() const;

    void setMiddle(TreeNode *middle);

    TreeNode *getRight() const;

    void setRight(TreeNode *right);

    TreeNode *getNext() const;

    void setNext(TreeNode *next);

};


#endif //CMMCOMPILER_TREENODE_H
