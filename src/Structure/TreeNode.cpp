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
        {ARRAY,        "ARRAY"},
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
    treeNodeType = type;
    value = t->getValue();
    line = t->getLine();
    dataType = t->getTagName();
    child = std::vector<TreeNode *>();
}

TreeNode::~TreeNode() {
    for (auto i:child) {
        delete i;
        i = nullptr;
    }
    child.clear();

}

void TreeNode::toString() {
    for (int i = 0; i < level; i++) {
        std::cout << "  ";
    }
    std::cout << ">";
    if (treeNodeType != NUL) {
        std::cout << treeNodeTypeName();
    }
    if (!value.empty()) {
        std::cout << "('" << value << "')" << std::endl;
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

void TreeNode::setChildLevels(int level) {
    TreeNode::level = level;
    for (auto &i:child) {
        i->setChildLevels(level + 1);
    }
}

int TreeNode::getLevel() const {
    return level;
}

void TreeNode::setDataType(const std::string &dataType) {
    TreeNode::dataType = dataType;
}

void TreeNode::setValue(const std::string &value) {
    TreeNode::value = value;
}

void TreeNode::setLine(int line) {
    TreeNode::line = line;
}

const std::string &TreeNode::getDataType() const {
    return dataType;
}

const std::string &TreeNode::getValue() const {
    return value;
}

int TreeNode::getLine() const {
    return line;
}
