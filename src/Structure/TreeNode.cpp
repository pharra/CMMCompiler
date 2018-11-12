//
// Created by chasi on 2018/11/12.
//

#include "Structure/TreeNode.h"
#include "TreeNode.h"


TreeNode *TreeNode::getLeft() const {
    return left;
}

void TreeNode::setLeft(TreeNode *left) {
    TreeNode::left = left;
}

TreeNode *TreeNode::getMiddle() const {
    return middle;
}

void TreeNode::setMiddle(TreeNode *middle) {
    TreeNode::middle = middle;
}

TreeNode *TreeNode::getRight() const {
    return right;
}

void TreeNode::setRight(TreeNode *right) {
    TreeNode::right = right;
}

TreeNode *TreeNode::getNext() const {
    return next;
}

void TreeNode::setNext(TreeNode *next) {
    TreeNode::next = next;
}

TreeNode::TreeNode(Token *t, TreeNodeType type) {
    token = t;
    treeNodeType = type;
    left = nullptr;
    middle = nullptr;
    right = nullptr;
    next = nullptr;
}

TreeNode::~TreeNode() {
    if (token != nullptr) {
        delete token;
        token = nullptr;
    }
    if (left != nullptr) {
        delete left;
        left = nullptr;
    }
    if (middle != nullptr) {
        delete middle;
        middle = nullptr;
    }
    if (right != nullptr) {
        delete right;
        right = nullptr;
    }
    if (next != nullptr) {
        delete next;
        next = nullptr;
    }
}

TokenTag TreeNode::getDataType() const {
    if(token != nullptr){
        return token->getTag();
    } else return UNDEFINED;
}

std::string TreeNode::getValue() const {
    if(token != nullptr){
        return token->getValue();
    } else return nullptr;
}

int TreeNode::getLine() const {
    if(token != nullptr){
        return token->getLine();
    } else return -1;
}
