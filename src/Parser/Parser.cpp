//
// Created by chasi on 2018/11/11.
//

#include "Parser.h"

Parser::Parser(std::string path) {
    lexer = new Lexer(std::move(path));
    nextToken = nullptr;
    nextNextToken = nullptr;
}

Parser::~Parser() {
    delete lexer;
    lexer = nullptr;

    for (auto i:errorLists) {
        delete i;
        i = nullptr;
    }
    errorLists.clear();

    for (auto i:tokens) {
        delete i;
        i = nullptr;
    }
    tokens.clear();

    for (auto i:unParsered) {
        delete i;
        i = nullptr;
    }
    unParsered.clear();
}

std::vector<TreeNode *> Parser::analyse() {
    getNextToken();
    try {
        while (getNextTokenType() != Token::END) {
            TreeNode *node = parseStmt();
            node->setChildLevels(0);
            treeNodeVec.push_back(node);
        }
    } catch (ParserException &pe) {
        std::cout << pe.message() << '\n';
    }
    for (auto i : errorLists) {
        std::cout << i->message() << '\n';
    }

    for (auto i : unParsered) {
        i->toString();
    }
    return treeNodeVec;
}

void Parser::getNextToken() {
    if (nextToken == nullptr && nextNextToken == nullptr) {
        nextToken = lexer->getNext();
        nextNextToken = lexer->getNext();
    } else {
        tokens.push_back(nextToken);
        nextToken = nextNextToken;
        nextNextToken = lexer->getNext();
    }
    if (getNextTokenType() == Token::MUL_NOTE || getNextTokenType() == Token::LINE_NOTE) {
        getNextToken();
    }
    while (getNextNextTokenType() == Token::MUL_NOTE || getNextNextTokenType() == Token::LINE_NOTE) {
        nextNextToken = lexer->getNext();
    }
    /*if (getNextTokenType() == Token::END) {
        throw ParserException("In file's end", nextToken->getLine(), ParserException::FILE_END);
    }*/
}


Token::TokenTag Parser::getNextTokenType() {
    return nextToken->getTag();
}

Token::TokenTag Parser::getNextNextTokenType() {
    return nextNextToken->getTag();
}

Token *Parser::popNextToken(Token::TokenTag type) {
    Token *tmp;
    ParserException *exception;
    if (getNextTokenType() == Token::END) {
        exception = new ParserException("in file's end && no next token", nextToken->getLine(),
                                        ParserException::FILE_END);
        errorLists.push_back(exception);
        return nullptr;
    }
    if (type == getNextTokenType()) {
        if (nextToken->isIsError()) {
            exception = new ParserException("Invalid Token: " + nextToken->getErrorMessage(), nextToken->getLine(),
                                            ParserException::TOKEN_ERROR);
            errorLists.push_back(exception);
        }
        tmp = nextToken;
        getNextToken();
        return tmp;
    } else {
        exception = new ParserException(
                "expected:" + Token::tokenTagMap.find(type)->second + " but: " + nextToken->getValue(),
                nextToken->getLine(),
                ParserException::NO_EXPECTED_TOKEN);
        errorLists.push_back(exception);
        tmp = new Token(type, "ERROR", nextToken->getLine(), nextToken->getColumn(),
                        "expected:" + Token::tokenTagMap.find(type)->second,
                        true);
        tokens.push_back(tmp);
        return tmp;
    }
}

Token *Parser::popNextToken(Token::TokenTag *types, int size) {
    Token *tmp;
    ParserException *exception;
    if (getNextTokenType() == Token::END) {
        exception = new ParserException("in file's end && no next token", nextToken->getLine(),
                                        ParserException::FILE_END);
        errorLists.push_back(exception);
        return nullptr;
    }
    for (int i = 0; i < size; i++) {
        if (types[i] == getNextTokenType()) {
            if (nextToken->isIsError()) {
                exception = new ParserException("Invalid Token: " + nextToken->getErrorMessage(), nextToken->getLine(),
                                                ParserException::TOKEN_ERROR);
                errorLists.push_back(exception);
            }
            tmp = nextToken;
            getNextToken();
            return tmp;
        }
    }

    exception = new ParserException("need expected tokens but: " + nextToken->getValue(), nextToken->getLine(),
                                    ParserException::NO_EXPECTED_TOKEN);
    errorLists.push_back(exception);
    tmp = new Token(types[0], "ERROR", nextToken->getLine(), nextToken->getColumn(),
                    "need expected tokens",
                    true);
    tokens.push_back(tmp);
    return tmp;
}

TreeNode *Parser::parseCharacter(Token::TokenTag type) {
    auto *node = new TreeNode(TreeNode::CHARACTER, popNextToken(type));
    return node;
}

TreeNode *Parser::parseStmt() {
    Token::TokenTag nextTokenType = getNextTokenType();
    switch (nextTokenType) {
        case Token::IF:
            return parseIfStmt();
        case Token::WHILE:
            return parseWhileStmt();
        case Token::FOR:
            return parseForStmt();
        case Token::READ:
            return parseReadStmt();
        case Token::WRITE:
            return parseWriteStmt();
        case Token::INT:
        case Token::REAL:
        case Token::CHAR:
            return parseDeclareStmt();
        case Token::LEFT_BOUNDER:
            return parseStmtBlock();
        case Token::IDENTIFIER:
            if (getNextNextTokenType() == Token::LEFT_BRA)
                return parseFunctionCall();
            else
                return parseAssignStmt();
        case Token::RETURN:
            return parseReturn();
        case Token::CLASS:
            return parseClassDeclare();
        case Token::NEW:
            return parseNewStmt();
        case Token::END:
            return nullptr;
        default:
            errorLists.push_back(
                    new ParserException("no expected Token", nextToken->getLine(), ParserException::NO_EXPECTED_TOKEN));
            unParsered.push_back(stepUntilTokenSEMI());
            return parseStmt();
    }
}

TreeNode *Parser::parseClassDeclare() {
    auto *node = new TreeNode(TreeNode::CLASS_DECLARE, popNextToken(Token::CLASS));
    TreeNode *t = parseVariableName();
    const std::string &newClass = t->getValue();
    lexer->pushNewClass(newClass);
    // node->push_back(t);
    node->push_back(parseCharacter(Token::LEFT_BRA));
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    auto *blockNode = new TreeNode(TreeNode::BLOCK_STMT);
    blockNode->push_back(parseCharacter(Token::LEFT_BOUNDER));
    while (getNextTokenType() != Token::RIGHT_BOUNDER) {
        if (getNextTokenType() == Token::CLASS_NAME && nextToken->getValue() == newClass &&
            getNextNextTokenType() == Token::LEFT_BRA) {
            auto *classConstructorNode = parseFunctionDeclare(true);
            classConstructorNode->setDataTypeValue(newClass);
            node->push_back(classConstructorNode);
            classConstructorNode->push_back(parseStmtBlock());
        }
        blockNode->push_back(parseDeclareStmt());
    }
    blockNode->push_back(parseCharacter(Token::RIGHT_BOUNDER));
    node->push_back(blockNode);
    return node;
}

TreeNode *Parser::parseIfStmt() {
    auto *node = new TreeNode(TreeNode::IF_STMT, popNextToken(Token::IF));

    node->push_back(parseCharacter(Token::LEFT_BRA));
    node->push_back(parseExp());
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    TreeNode *tmp = parseStmt();
    if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
        auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
        blockStmt->push_back(tmp);
        tmp = blockStmt;
    }
    node->push_back(tmp);
    if (getNextTokenType() == Token::ELSE) {
        node->push_back(parseCharacter(Token::ELSE));
        tmp = parseStmt();
        if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
            auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
            blockStmt->push_back(tmp);
            tmp = blockStmt;
        }
        node->push_back(tmp);
    }
    return node;
}

TreeNode *Parser::parseWhileStmt() {
    auto *node = new TreeNode(TreeNode::WHILE_STMT, popNextToken(Token::WHILE));
    node->push_back(parseCharacter(Token::LEFT_BRA));
    node->push_back(parseExp());
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    TreeNode *tmp = parseStmt();
    if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
        auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
        blockStmt->push_back(tmp);
        tmp = blockStmt;
    }
    node->push_back(tmp);
    return node;
}

TreeNode *Parser::parseForStmt() {
    auto *node = new TreeNode(TreeNode::FOR_STMT, popNextToken(Token::FOR));
    node->push_back(parseCharacter(Token::LEFT_BRA));
    Token::TokenTag type[] = {Token::INT, Token::REAL, Token::CHAR, Token::CLASS_NAME};
    TreeNode *first = nullptr;
    if (checkNextTokenType(type, 4)) {
        first = parseDeclareStmt();
    } else {
        first = parseAssignStmt();
    }
    node->push_back(first);
    node->push_back(parseExp());
    node->push_back(parseCharacter(Token::SEMI));
    node->push_back(parseAssignStmt(true));
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    TreeNode *tmp = parseStmt();
    if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
        auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
        blockStmt->push_back(tmp);
        tmp = blockStmt;
    }
    node->push_back(tmp);
    return node;
}

TreeNode *Parser::parseReadStmt() {
    auto *node = new TreeNode(TreeNode::READ_STMT, popNextToken(Token::READ));
    node->push_back(parseCharacter(Token::LEFT_BRA));
    auto varNode = parseVariableName();
    node->push_back(varNode);
    if (getNextTokenType() == Token::LEFT_INDEX) {
        varNode->setIsArray(true);
        varNode->push_back(parseCharacter(Token::LEFT_INDEX));
        varNode->push_back(parseExp());
        varNode->push_back(parseCharacter(Token::RIGHT_INDEX));
    }
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    node->push_back(parseCharacter(Token::SEMI));
    return node;
}

TreeNode *Parser::parseWriteStmt() {
    auto *node = new TreeNode(TreeNode::WRITE_STMT, popNextToken(Token::WRITE));
    node->push_back(parseCharacter(Token::LEFT_BRA));
    node->push_back(parseExp());
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    node->push_back(parseCharacter(Token::SEMI));
    return node;
}

TreeNode *Parser::parseNewStmt() {
    auto *node = new TreeNode(TreeNode::NEW_STMT, popNextToken(Token::NEW));
    Token::TokenTag type[] = {Token::INT, Token::REAL, Token::CHAR, Token::CLASS_NAME};
    auto *typeToken = popNextToken(type, 4);
    node->setDataType(typeToken->getTag());
    node->setDataTypeValue(typeToken->getValue());
    node->push_back(parseCharacter(Token::LEFT_INDEX));
    node->push_back(parseExp());
    node->push_back(parseCharacter(Token::RIGHT_INDEX));
    // node->push_back(parseCharacter(Token::SEMI));
    return node;
}

TreeNode *Parser::parseDeclareStmt(bool isParseFun) {
    Token::TokenTag type[] = {Token::INT, Token::REAL, Token::CHAR, Token::CLASS_NAME};
    Token *typeToken = popNextToken(type, 4);
    Token::TokenTag dataType = typeToken->getTag();
    const std::string &dataTypeValue = typeToken->getValue();
    bool array = false;
    if (getNextTokenType() == Token::LEFT_INDEX) { // 数组声明
        popNextToken(Token::LEFT_INDEX);
        popNextToken(Token::RIGHT_INDEX);
        array = true;
    }
    auto *node = new TreeNode(TreeNode::DECLARE_STMT);

    if (getNextNextTokenType() == Token::LEFT_BRA)    //函数声明
    {
        TreeNode *funNode = parseFunctionDeclare();
        funNode->setDataType(dataType);
        funNode->setIsArray(array);
        funNode->setDataTypeValue(dataTypeValue);
        node->push_back(funNode);
        node->push_back(parseStmtBlock());
    } else {        //声明变量
        TreeNode *varNode;
        varNode = parseVariableName();
        varNode->setDataType(dataType);
        varNode->setIsArray(array);
        varNode->setDataTypeValue(dataTypeValue);
        node->push_back(varNode);
        if (getNextTokenType() == Token::ASSIGN) {
            node->push_back(parseCharacter(Token::ASSIGN));
            if (!varNode->isArray()) {
                node->push_back(parseExp());
            } else {    //声明时初始化数组
                if (getNextTokenType() == Token::CHAR_VALUE)   //如果是char[] = ""
                {
                    node->push_back(parseLiteral());
                } else if (getNextTokenType() == Token::NEW) {
                    node->push_back(parseNewStmt());
                } else {
                    node->push_back(parseCharacter(Token::LEFT_BOUNDER));
                    if (getNextTokenType() == Token::RIGHT_BOUNDER) {
                        node->push_back(parseCharacter(Token::RIGHT_BOUNDER));
                    } else {
                        auto *temp = new TreeNode(TreeNode::ARRAY);
                        node->push_back(temp);
                        temp->push_back(parseExp());
                        while (getNextTokenType() == Token::COMMA) {
                            popNextToken(Token::COMMA);
                            temp->push_back(parseExp());
                        }
                        node->push_back(parseCharacter(Token::RIGHT_BOUNDER));
                    }
                }
            }
        }
        if (isParseFun) {
        } else {
            node->push_back(parseCharacter(Token::SEMI));
        }
    }
    return node;
}

TreeNode *Parser::parseAssignStmt(bool isParseFor) {
    auto *node = new TreeNode(TreeNode::ASSIGN_STMT);
    auto *varNode = parseVariableName();
    node->push_back(varNode);
    if (getNextTokenType() == Token::LEFT_INDEX) {
        varNode->setIsArray(true);
        varNode->push_back(parseCharacter(Token::LEFT_INDEX));
        varNode->push_back(parseExp());
        varNode->push_back(parseCharacter(Token::RIGHT_INDEX));
    }
    node->push_back(parseCharacter(Token::ASSIGN));
    node->push_back(parseExp());
    if (isParseFor) {
    } else {
        node->push_back(parseCharacter(Token::SEMI));
    }
    return node;
}

TreeNode *Parser::parseStmtBlock() {
    auto *node = new TreeNode(TreeNode::BLOCK_STMT);
    TreeNode *temp;
    node->push_back(parseCharacter(Token::LEFT_BOUNDER));
    while (getNextTokenType() != Token::RIGHT_BOUNDER) {
        temp = parseStmt();
        if (temp == nullptr) {
            break;
        }
        node->push_back(temp);
    }
    node->push_back(parseCharacter(Token::RIGHT_BOUNDER));
    return node;
}


TreeNode *Parser::parseExp() {
    auto *node = new TreeNode(TreeNode::EXP);
    node->setDataType(Token::LOGIC_EXP);
    TreeNode *leftNode = parseAddtiveExp();
    Token::TokenTag types[6] = {Token::EQL, Token::NOT_EQL, Token::GRT, Token::GRT_EQL, Token::LES, Token::LES_EQL};
    if (checkNextTokenType(types, 6)) {
        node->push_back(leftNode);
        node->push_back(parseLogicOp());
        node->push_back(parseAddtiveExp());
        return node;
    } else {
        return leftNode;
    }
}

bool Parser::checkNextTokenType(Token::TokenTag *types, int size) {
    Token::TokenTag nextType = getNextTokenType();
    for (int i = 0; i < size; ++i) {
        if (nextType == types[i]) {
            return true;
        }
    }
    return false;
}

TreeNode *Parser::parseAddtiveExp() {
    auto *node = new TreeNode(TreeNode::EXP);
    node->setDataType(Token::ADDTIVE_EXP);
    TreeNode *leftNode = parseTerm();
    Token::TokenTag types[2] = {Token::PLUS, Token::MINUS};
    if (checkNextTokenType(types, 2)) {
        node->push_back(leftNode);
        node->push_back(parseAddtiveOp());
        node->push_back(parseAddtiveExp());
        return node;
    } else {
        return leftNode;
    }
}

TreeNode *Parser::parseTerm() {
    auto *node = new TreeNode(TreeNode::EXP);
    node->setDataType(Token::TERM_EXP);
    TreeNode *leftNode = parseFactor();
    Token::TokenTag types[2] = {Token::MUL, Token::DIV};
    if (checkNextTokenType(types, 2)) {
        node->push_back(leftNode);
        node->push_back(parseMutiplyOp());
        node->push_back(parseTerm());
        return node;
    } else {
        return leftNode;
    }
}

TreeNode *Parser::parseFactor() {
    auto *node = new TreeNode(TreeNode::FACTOR);
    switch (getNextTokenType()) {
        case Token::CHAR_VALUE:
        case Token::INT_VALUE:
        case Token::REAL_VALUE:
            node->push_back(parseLiteral());
            break;
        case Token::LEFT_BRA:
            node->push_back(parseCharacter(Token::LEFT_BRA));
            node = parseExp();
            node->push_back(parseCharacter(Token::RIGHT_BRA));
            break;
        case Token::MINUS:
        case Token::PLUS:
            node->push_back(parseAddtiveOp());
            node->push_back(parseTerm());
            break;
        default:
            if (getNextNextTokenType() == Token::LEFT_BRA)
                node->push_back(parseFunctionCall(false));
            else
                node->push_back(parseVariableName());
            break;
    }
    return node;
}

TreeNode *Parser::parseLiteral() {
    auto *node = new TreeNode(TreeNode::LITERAL, popNextToken(getNextTokenType()));
    return node;
}


TreeNode *Parser::parseLogicOp() {
    Token::TokenTag types[6] = {Token::EQL, Token::NOT_EQL, Token::GRT, Token::GRT_EQL, Token::LES, Token::LES_EQL};
    auto *node = new TreeNode(TreeNode::OP, popNextToken(types, 6));
    return node;
}


TreeNode *Parser::parseAddtiveOp() {
    Token::TokenTag types[2] = {Token::PLUS, Token::MINUS};
    auto *node = new TreeNode(TreeNode::OP, popNextToken(types, 2));
    return node;
}


TreeNode *Parser::parseMutiplyOp() {
    Token::TokenTag types[2] = {Token::MUL, Token::DIV};
    auto *node = new TreeNode(TreeNode::OP, popNextToken(types, 2));
    return node;
}


TreeNode *Parser::parseVariableName() {
    auto *node = new TreeNode(TreeNode::VAR, popNextToken(Token::IDENTIFIER));
    return node;
}


TreeNode *Parser::parseFunctionDeclare(bool isConstructor) {
    TreeNode *node = nullptr;
    if (isConstructor) {
        node = new TreeNode(TreeNode::CLASS_CONSTRUCTOR, popNextToken(Token::CLASS_NAME));
    } else {
        node = new TreeNode(TreeNode::FUNCTION_DECLARE, popNextToken(Token::IDENTIFIER));
    }
    node->push_back(parseCharacter(Token::LEFT_BRA));
    Token::TokenTag types[4] = {Token::INT, Token::REAL, Token::CHAR, Token::CLASS_NAME};
    auto *params = new TreeNode(TreeNode::PARAMS_STMT);
    _dataSize argIndex = 1;
    if (checkNextTokenType(types, 4)) {
        params->push_back(parseDeclareStmt(true));
        while (getNextTokenType() != Token::RIGHT_BRA) {
            params->push_back(parseCharacter(Token::COMMA));
            if (!checkNextTokenType(types, 4)) {
                errorLists.push_back(new ParserException("Invalid declare function params stmt", nextToken->getLine(),
                                                         ParserException::NO_EXPECTED_TOKEN));
                Token::TokenTag forStep[] = {Token::COMMA, Token::RIGHT_BRA};
                unParsered.push_back(stepUntilToken(forStep, 2));
            }
            auto* arg = parseDeclareStmt(true);
            auto * regNode = new TreeNode(TreeNode::REG);
            regNode->setValue("@ebp-" + std::to_string(argIndex));
            arg->push_back(regNode);
            params->push_back(arg);
        }
    }
    node->push_back(params);
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    return node;
}


TreeNode *Parser::parseFunctionCall(bool isStmt) {
    auto *node = new TreeNode(TreeNode::FUNCTION_CALL, popNextToken(Token::IDENTIFIER));
    node->push_back(parseCharacter(Token::LEFT_BRA));
    auto *params = new TreeNode(TreeNode::PARAMS_STMT);
    if (getNextTokenType() != Token::RIGHT_BRA) {
        params->push_back(parseExp());
        while (getNextTokenType() != Token::RIGHT_BRA) {
            params->push_back(parseCharacter(Token::COMMA));
            params->push_back(parseExp());
        }
    }
    node->push_back(params);
    node->push_back(parseCharacter(Token::RIGHT_BRA));
    if (isStmt) {
        node->push_back(parseCharacter(Token::SEMI));
    }
    return node;
}


TreeNode *Parser::parseReturn() {
    auto *node = new TreeNode(TreeNode::RETURN_STMT, popNextToken(Token::RETURN));
    node->push_back(parseExp());
    node->push_back(parseCharacter(Token::SEMI));
    return node;
}

TreeNode *Parser::stepUntilTokenSEMI() {
    auto *root = new TreeNode(TreeNode::ERROR_STEP);
    while (getNextTokenType() != Token::SEMI) {
        Token *tmp = popNextToken(getNextTokenType());
        if (tmp == nullptr) {
            break;
        }
        auto *t = new TreeNode(TreeNode::NUL, tmp);
        root->push_back(t);
    }
    root->push_back(new TreeNode(TreeNode::NUL, popNextToken(Token::SEMI)));
    return root;
}

TreeNode *Parser::stepUntilToken(Token::TokenTag *types, int size) {
    auto *root = new TreeNode(TreeNode::ERROR_STEP);
    Token *tmp;
    bool isToken = false;
    while (true) {
        for (int i = 0; i < size; i++) {
            if (types[i] == getNextTokenType()) {
                isToken = true;
            }
        }
        if (isToken) {
            break;
        }
        tmp = popNextToken(getNextTokenType());
        if (tmp == nullptr) {
            break;
        }
        auto *t = new TreeNode(TreeNode::NUL, tmp);
        root->push_back(t);
    }
    return root;
}

std::vector<TreeNode *> Parser::clearCharaterNode(std::vector<TreeNode *> &nodeVec) {
    for (auto &i:nodeVec) {
        i->removeCharacter();
    }
    return nodeVec;
}
