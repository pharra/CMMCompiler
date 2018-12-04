//
// Created by chasi on 2018/11/12.
//

#include "Structure/TreeNode.h"
#include "TreeNode.h"

std::map<TreeNode::TreeNodeType, std::string> TreeNode::treeNodeTypeMap = {
        {NUL,          "NUL"},
        {CHARACTER,    "CHARACTER"},
        {IF_STMT,      "IF_STMT"},
        {WHILE_STMT,   "WHILE_STMT"},
        {FOR_STMT,     "FOR_STMT"},
        {READ_STMT,    "READ_STMT"},
        {WRITE_STMT,   "WRITE_STMT"},
        {DECLARE_STMT, "DECLARE_STMT"},
        {ASSIGN_STMT,  "ASSIGN_STMT"},
        {BLOCK_STMT,   "BLOCK_STMT"},
        {RETURN_STMT,  "RETURN_STMT"},
        {EXP,          "EXP"},
        {VAR,          "VAR"},
        {FUNDECLARE,   "FUNDECLARE"},
        {FUNCALL,      "FUNCALL"},
        {OP,           "OP"},
        {FACTOR,       "FACTOR"},
        {LITERAL,      "LITERAL"},
        {ERRORSTEP,    "ERRORSTEP"}
};

TreeNode::TreeNode(TreeNodeType type, Token *t) {
    token = t;
    treeNodeType = type;
    next = nullptr;
    child = std::vector<TreeNode *>();
}

TreeNode::~TreeNode() {
    delete token;
    token = nullptr;

    for (auto i:child) {
        delete i;
        i = nullptr;
    }
    child.clear();

    delete next;
    next = nullptr;

}

Token::TokenTag TreeNode::getDataType() const {
    if (token != nullptr) {
        return token->getTag();
    } else return Token::UNDEFINED;
}

std::string TreeNode::getValue() const {
    if (token != nullptr) {
        return token->getValue();
    } else return nullptr;
}

int TreeNode::getLine() const {
    if (token != nullptr) {
        return token->getLine();
    } else return -1;
}


TreeNode *TreeNode::getNext() const {
    return next;
}

void TreeNode::setNext(TreeNode *next) {
    delete TreeNode::next;
    TreeNode::next = next;
}

void TreeNode::toString() {
    for (int i = 0; i < level; i++) {
        std::cout << "  ";
    }
    std::cout << ">";
    if (treeNodeType != NUL) {
        std::cout << treeNodeTypeName();
    }
    if (token) {
        std::cout << "('" << token->getValue() << "')" << std::endl;
    } else {
        std::cout << std::endl;
    }
    for (auto &i : child) {
        i->toString();
    }
    for (auto &i : child) {
        if (i->getNext() != nullptr) {
            i->getNext()->toString();
        }
    }
    if (next != nullptr) {
        next->toString();
    }
}

std::string TreeNode::treeNodeTypeName() const {
    return treeNodeTypeMap.find(treeNodeType)->second;
}

std::vector<TreeNode *> TreeNode::getChild() const {
    return child;
}

int TreeNode::getLevel() const {
    return level;
}

void TreeNode::setLevel(int level) {
    TreeNode::level = level;
}

void TreeNode::push_back(TreeNode *node) {
    child.push_back(node);
}

Token *TreeNode::getToken() const {
    return token;
}

TreeNode::TreeNodeType TreeNode::getTreeNodeType() const {
    return treeNodeType;
}
