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
        {ERRORSTEP,    "ERRORSTEP"},
        {PARAMS_STMT,  "PARAMS_STMT"},
};

TreeNode::TreeNode(TreeNodeType type, Token *t) {
    token = t->clone();
    treeNodeType = type;
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

}

Token::TokenTag TreeNode::getDataType() const {
    return token->getTag();
}

std::string TreeNode::getValue() const {
    return token->getValue();
}

int TreeNode::getLine() const {
    return token->getLine();
}

void TreeNode::toString() {
    for (int i = 0; i < level; i++) {
        std::cout << "  ";
    }
    std::cout << ">";
    if (treeNodeType != NUL) {
        std::cout << treeNodeTypeName();
    }
    if (token->getTag() != Token::UNDEFINED) {
        std::cout << "('" << token->getValue() << "')" << std::endl;
    } else {
        std::cout << std::endl;
    }
    for (auto &i : child) {
        i->toString();
    }
}

std::string TreeNode::treeNodeTypeName() const {
    return treeNodeTypeMap.find(treeNodeType)->second;
}

std::vector<TreeNode *> TreeNode::getChild() const {
    return child;
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

void TreeNode::removeCharacter() {
    for (auto i = child.begin(); i != child.end();) {
        (*i)->removeCharacter();
        if ((*i)->getTreeNodeType() == CHARACTER) {
            i = child.erase(i);
        } else {
            i++;
        }
    }
}

void TreeNode::setToken(Token *t) {
    token = t;
}

void TreeNode::setChildLevels(int level) {
    TreeNode::level = level;
    for (auto &i:child) {
        i->setChildLevels(level + 1);
    }
}

int TreeNode::getLevel() const {
    return level;
}
