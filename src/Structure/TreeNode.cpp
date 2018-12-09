//
// Created by chasi on 2018/11/12.
//

#include "Structure/TreeNode.h"
#include "TreeNode.h"

std::map<TreeNode::TreeNodeType, std::string> TreeNode::treeNodeTypeMap = {
        {NUL,               "NUL"},
        {CHARACTER,         "CHARACTER"},
        {IF_STMT,           "IF_STMT"},
        {WHILE_STMT,        "WHILE_STMT"},
        {FOR_STMT,          "FOR_STMT"},
        {READ_STMT,         "READ_STMT"},
        {WRITE_STMT,        "WRITE_STMT"},
        {DECLARE_STMT,      "DECLARE_STMT"},
        {ASSIGN_STMT,       "ASSIGN_STMT"},
        {BLOCK_STMT,        "BLOCK_STMT"},
        {RETURN_STMT,       "RETURN_STMT"},
        {NEW_STMT,          "NEW_STMT"},
        {CLASS_DECLARE,     "CLASS_DECLARE"},
        {CLASS_CONSTRUCTOR, "CLASS_CONSTRUCTOR"},
        {EXP,               "EXP"},
        {ARRAY,             "ARRAY"},
        {VAR,               "VAR"},
        {FUNCTION_DECLARE,  "FUNCTION_DECLARE"},
        {FUNCTION_CALL,     "FUNCTION_CALL"},
        {OP,                "OP"},
        {FACTOR,            "FACTOR"},
        {LITERAL,           "LITERAL"},
        {ERROR_STEP,        "ERROR_STEP"},
        {PARAMS_STMT,       "PARAMS_STMT"},
};

TreeNode::TreeNode(TreeNodeType type, Token *t) {
    treeNodeType = type;
    value = t->getValue();
    line = t->getLine();
    dataType = t->getTag();
    child = std::vector<TreeNode *>();
    _isArray = false;
}

TreeNode::~TreeNode() {
    for (auto i:child) {
        delete i;
        i = nullptr;
    }
    child.clear();

}

const std::string TreeNode::toString() {
    std::stringstream stringStream;
    for (int i = 0; i < level; i++) {
        stringStream << "  ";
    }
    stringStream << ">";
    if (treeNodeType != NUL) {
        stringStream << treeNodeTypeName();
    }
    if (!value.empty()) {
        stringStream << "(name: " << value << " , type: ";
        if (dataType == Token::CLASS_NAME) {
            stringStream << dataTypeValue;
        } else {
            stringStream << Token::tokenTagMap.find(dataType)->second;
        }
        if (_isArray) {
            stringStream << "[]";
        }
        stringStream << ")"
                     << '\n';
    } else {
        stringStream << '\n';
    }
    for (auto &i : child) {
        stringStream << i->toString();
    }
    return stringStream.str();
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

void TreeNode::setValue(const std::string &value) {
    TreeNode::value = value;
}

void TreeNode::setLine(int line) {
    TreeNode::line = line;
}

const std::string &TreeNode::getValue() const {
    return value;
}

int TreeNode::getLine() const {
    return line;
}

Token::TokenTag TreeNode::getDataType() const {
    return dataType;
}

void TreeNode::setDataType(Token::TokenTag dataType) {
    TreeNode::dataType = dataType;
}

bool TreeNode::isArray() const {
    return _isArray;
}

void TreeNode::setIsArray(bool isArray) {
    TreeNode::_isArray = isArray;
}

const std::string &TreeNode::getDataTypeValue() const {
    return dataTypeValue;
}

void TreeNode::setDataTypeValue(const std::string &dataTypeValue) {
    TreeNode::dataTypeValue = dataTypeValue;
}
