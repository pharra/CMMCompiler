//
// Created by chasi on 2018/11/11.
//

#include "Parser.h"

Parser::Parser(std::string path) {
    lexer = new Lexer(std::move(path));
    nextToken = nullptr;
    nextNextToken = nullptr;
//    errorLists = std::list<ParserException *>();
//    ignoreTokens = std::list<Token *>();
//    unParsered = std::list<TreeNode *>();
}

Parser::~Parser() {
    delete lexer;
    lexer = nullptr;

    for (auto i:errorLists) {
        delete i;
        i = nullptr;
    }
    errorLists.clear();

    for (auto i:ignoreTokens) {
        delete i;
        i = nullptr;
    }
    ignoreTokens.clear();

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
            TreeNode *node = parseStmt(0);
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
        nextToken = nextNextToken;
        nextNextToken = lexer->getNext();
    }
    if (getNextTokenType() == Token::MUL_NOTE || getNextTokenType() == Token::LINE_NOTE) {
        ignoreTokens.push_back(nextToken);
        getNextToken();
    }
    while (getNextNextTokenType() == Token::MUL_NOTE || getNextNextTokenType() == Token::LINE_NOTE) {
        ignoreTokens.push_back(nextToken);
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
        exception = new ParserException("expected:" + Token::tokenTagMap.find(type)->second, nextToken->getLine(),
                                        ParserException::NO_EXPECTED_TOKEN);
        errorLists.push_back(exception);
        tmp = new Token(type, "ERROR", nextToken->getLine(), nextToken->getColumn(),
                        "expected:" + Token::tokenTagMap.find(type)->second,
                        true);
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

    exception = new ParserException("need expected tokens", nextToken->getLine(),
                                    ParserException::NO_EXPECTED_TOKEN);
    errorLists.push_back(exception);
    tmp = new Token(types[0], "ERROR", nextToken->getLine(), nextToken->getColumn(),
                    "need expected tokens",
                    true);
    return tmp;
}

TreeNode *Parser::parseCharacter(Token::TokenTag type, int level) {
    auto *node = new TreeNode(TreeNode::CHARACTER, popNextToken(type));
    node->setLevel(level);
    return node;
}

TreeNode *Parser::parseStmt(int level) {
    Token::TokenTag nextTokenType = getNextTokenType();
    switch (nextTokenType) {
        case Token::IF:
            return parseIfStmt(level);
        case Token::WHILE:
            return parseWhileStmt(level);
        case Token::FOR:
            return parseForStmt(level);
        case Token::READ:
            return parseReadStmt(level);
        case Token::WRITE:
            return parseWriteStmt(level);
        case Token::INT:
        case Token::REAL:
        case Token::STRING:
            return parseDeclareStmt(level);
        case Token::LEFT_BOUNDER:
            return parseStmtBlock(level);
        case Token::IDENTIFIER:
            if (getNextNextTokenType() == Token::LEFT_BRA)
                return parseFunctionCall(level);
            else
                return parseAssignStmt(level);
        case Token::RETURN:
            return parseReturn(level);
        case Token::END:
            return nullptr;
        default:
            errorLists.push_back(
                    new ParserException("no expected Token", nextToken->getLine(), ParserException::NO_EXPECTED_TOKEN));
            unParsered.push_back(stepUntilTokenSEMI(level));
            return parseStmt(level);
    }
}

TreeNode *Parser::parseIfStmt(int level) {
    auto *node = new TreeNode(TreeNode::IF_STMT, popNextToken(Token::IF));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    TreeNode *tmp = parseStmt(level + 1);
    if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
        auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
        blockStmt->setLevel(tmp->getLevel());
        blockStmt->push_back(tmp);
        tmp->setLevel(tmp->getLevel() + 1);
        tmp = blockStmt;
    }
    node->push_back(tmp);
    if (getNextTokenType() == Token::ELSE) {
        node->push_back(parseCharacter(Token::ELSE, level + 1));
        TreeNode *tmp = parseStmt(level + 1);
        if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
            auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
            blockStmt->setLevel(tmp->getLevel());
            blockStmt->push_back(tmp);
            tmp->setLevel(tmp->getLevel() + 1);
            tmp = blockStmt;
        }
        node->push_back(tmp);
    }
    return node;
}

TreeNode *Parser::parseWhileStmt(int level) {
    auto *node = new TreeNode(TreeNode::WHILE_STMT, popNextToken(Token::WHILE));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    TreeNode *tmp = parseStmt(level + 1);
    if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
        auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
        blockStmt->setLevel(tmp->getLevel());
        blockStmt->push_back(tmp);
        tmp->setLevel(tmp->getLevel() + 1);
        tmp = blockStmt;
    }
    node->push_back(tmp);
    return node;
}

TreeNode *Parser::parseForStmt(int level) {
    auto *node = new TreeNode(TreeNode::FOR_STMT, popNextToken(Token::FOR));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    node->push_back(parseAssignStmt(level + 1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::SEMI, level + 1));
    node->push_back(parseAssignStmt(level + 1, true));
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    TreeNode *tmp = parseStmt(level + 1);
    if (tmp->getTreeNodeType() != TreeNode::BLOCK_STMT) {
        auto *blockStmt = new TreeNode(TreeNode::BLOCK_STMT);
        blockStmt->setLevel(tmp->getLevel());
        blockStmt->push_back(tmp);
        tmp->setLevel(tmp->getLevel() + 1);
        tmp = blockStmt;
    }
    node->push_back(tmp);
    return node;
}

TreeNode *Parser::parseReadStmt(int level) {
    auto *node = new TreeNode(TreeNode::READ_STMT, popNextToken(Token::READ));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    node->push_back(parseVariableName(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    node->push_back(parseCharacter(Token::SEMI, level + 1));
    return node;
}

TreeNode *Parser::parseWriteStmt(int level) {
    auto *node = new TreeNode(TreeNode::WRITE_STMT, popNextToken(Token::WRITE));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    node->push_back(parseCharacter(Token::SEMI, level + 1));
    return node;
}

TreeNode *Parser::parseDeclareStmt(int level, bool isParseFun) {
    Token::TokenTag type[] = {Token::INT, Token::REAL, Token::STRING};
    auto *node = new TreeNode(TreeNode::DECLARE_STMT, popNextToken(type, 3));
    node->setLevel(level);
    if (getNextNextTokenType() == Token::LEFT_BRA)    //函数声明
    {
        TreeNode *funNode = parseFunctionDeclare(level + 1);
        node->push_back(funNode);
        node->push_back(parseStmtBlock(level + 1));
    } else {        //声明变量
        TreeNode *varNode;
        varNode = parseVariableName(level + 1);
        node->push_back(varNode);

        if (getNextTokenType() == Token::ASSIGN) {
            node->push_back(parseCharacter(Token::ASSIGN, level + 1));
            if (varNode->getChild().empty()) {
                node->push_back(parseExp(level + 1));
            }
        }
        if (isParseFun) {

        } else {
            node->push_back(parseCharacter(Token::SEMI, level + 1));
        }

    }
    return node;
}

TreeNode *Parser::parseAssignStmt(int level, bool isParseFor) {
    auto *node = new TreeNode(TreeNode::ASSIGN_STMT);
    node->setLevel(level);
    node->push_back(parseVariableName(level + 1));
    node->push_back(parseCharacter(Token::ASSIGN, level + 1));
    node->push_back(parseExp(level + 1));
    if (isParseFor) {

    } else {
        node->push_back(parseCharacter(Token::SEMI, level + 1));
    }
    return node;
}

TreeNode *Parser::parseStmtBlock(int level) {
    auto *node = new TreeNode(TreeNode::BLOCK_STMT);
    node->setLevel(level);
    TreeNode *temp;
    node->push_back(parseCharacter(Token::LEFT_BOUNDER, level + 1));
    while (getNextTokenType() != Token::RIGHT_BOUNDER) {
        temp = parseStmt(level + 1);
        if (temp == nullptr) {
            break;
        }
        node->push_back(temp);
    }
    node->push_back(parseCharacter(Token::RIGHT_BOUNDER, level + 1));
    return node;
}


TreeNode *Parser::parseExp(int level) {
    auto *node = new TreeNode(TreeNode::EXP);
    node->setLevel(level);
    TreeNode *leftNode = parseAddtiveExp(level + 1);
    Token::TokenTag types[6] = {Token::EQL, Token::NOT_EQL, Token::GRT, Token::GRT_EQL, Token::LES, Token::LES_EQL};
    if (checkNextTokenType(types, 6)) {
        node->push_back(leftNode);
        node->push_back(parseLogicOp(level + 1));
        node->push_back(parseAddtiveExp(level + 1));
        return node;
    } else {
        leftNode->setLevel(leftNode->getLevel() - 1);
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

TreeNode *Parser::parseAddtiveExp(int level) {
    auto *node = new TreeNode(TreeNode::EXP);
    node->setLevel(level);
    TreeNode *leftNode = parseTerm(level + 1);
    Token::TokenTag types[2] = {Token::PLUS, Token::MINUS};
    if (checkNextTokenType(types, 2)) {
        node->push_back(leftNode);
        node->push_back(parseAddtiveOp(level + 1));
        node->push_back(parseAddtiveExp(level + 1));
        return node;
    } else {
        leftNode->setLevel(leftNode->getLevel() - 1);
        return leftNode;
    }
}

TreeNode *Parser::parseTerm(int level) {
    auto *node = new TreeNode(TreeNode::EXP);
    node->setLevel(level);
    TreeNode *leftNode = parseFactor(level + 1);
    Token::TokenTag types[2] = {Token::MUL, Token::DIV};
    if (checkNextTokenType(types, 2)) {
        node->push_back(leftNode);
        node->push_back(parseMutiplyOp(level + 1));
        node->push_back(parseTerm(level + 1));
        return node;
    } else {
        leftNode->setLevel(leftNode->getLevel() - 1);
        return leftNode;
    }
}

TreeNode *Parser::parseFactor(int level) {
    auto *node = new TreeNode(TreeNode::FACTOR);
    node->setLevel(level);
    switch (getNextTokenType()) {
        case Token::CHARS:
        case Token::NUM:
            node->push_back(parseLiteral(level + 1));
            break;
        case Token::LEFT_BRA:
            node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
            node = parseExp(level + 1);
            node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
            break;
        case Token::MINUS:
            node->push_back(parseCharacter(Token::MINUS, level + 1));
            node->push_back(parseTerm(level + 1));
            break;
        case Token::PLUS:
            node->push_back(parseCharacter(Token::PLUS, level + 1));
            node->push_back(parseTerm(level + 1));
            break;
        default:
            if (getNextNextTokenType() == Token::LEFT_BRA)
                node->push_back(parseFunctionCall(level + 1, false));
            else
                node->push_back(parseVariableName(level + 1));
            break;
    }
    return node;
}

TreeNode *Parser::parseLiteral(int level) {
    auto *node = new TreeNode(TreeNode::LITERAL, popNextToken(getNextTokenType()));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseLogicOp(int level) {
    Token::TokenTag types[6] = {Token::EQL, Token::NOT_EQL, Token::GRT, Token::GRT_EQL, Token::LES, Token::LES_EQL};
    auto *node = new TreeNode(TreeNode::OP, popNextToken(types, 6));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseAddtiveOp(int level) {
    Token::TokenTag types[2] = {Token::PLUS, Token::MINUS};
    auto *node = new TreeNode(TreeNode::OP, popNextToken(types, 2));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseMutiplyOp(int level) {
    Token::TokenTag types[2] = {Token::MUL, Token::DIV};
    auto *node = new TreeNode(TreeNode::OP, popNextToken(types, 2));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseVariableName(int level) {
    auto *node = new TreeNode(TreeNode::VAR, popNextToken(Token::IDENTIFIER));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseFunctionDeclare(int level) {
    auto *node = new TreeNode(TreeNode::FUNDECLARE, popNextToken(Token::IDENTIFIER));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    Token::TokenTag types[3] = {Token::INT, Token::REAL, Token::CHARS};
    auto *params = new TreeNode(TreeNode::PARAMS_STMT);
    params->setLevel(level + 1);
    if (checkNextTokenType(types, 3)) {
        params->push_back(parseDeclareStmt(level + 2, true));
        while (getNextTokenType() != Token::RIGHT_BRA) {
            params->push_back(parseCharacter(Token::COMMA, level + 2));
            if (!checkNextTokenType(types, 3)) {
                errorLists.push_back(new ParserException("Invalid declare function params stmt", nextToken->getLine(),
                                                         ParserException::NO_EXPECTED_TOKEN));
                Token::TokenTag forStep[] = {Token::COMMA, Token::RIGHT_BRA};
                unParsered.push_back(stepUntilToken(forStep, 2, level));
            }
            params->push_back(parseDeclareStmt(level + 2, true));
        }
    }
    node->push_back(params);
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    return node;
}


TreeNode *Parser::parseFunctionCall(int level, bool isStmt) {
    auto *node = new TreeNode(TreeNode::FUNCALL, popNextToken(Token::IDENTIFIER));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    auto *params = new TreeNode(TreeNode::PARAMS_STMT);
    params->setLevel(level + 1);
    if (getNextTokenType() != Token::RIGHT_BRA) {
        params->push_back(parseExp(level + 2));
        while (getNextTokenType() != Token::RIGHT_BRA) {
            params->push_back(parseCharacter(Token::COMMA, level + 2));
            params->push_back(parseExp(level + 2));
        }
    }
    node->push_back(params);
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    if (isStmt) {
        node->push_back(parseCharacter(Token::SEMI, level + 1));
    }
    return node;
}


TreeNode *Parser::parseReturn(int level) {
    auto *node = new TreeNode(TreeNode::RETURN_STMT, popNextToken(Token::RETURN));
    node->setLevel(level);
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::SEMI, level + 1));
    return node;
}

TreeNode *Parser::stepUntilTokenSEMI(int level) {
    auto *root = new TreeNode(TreeNode::ERRORSTEP);
    root->setLevel(level);
    while (getNextTokenType() != Token::SEMI) {
        Token *tmp = popNextToken(getNextTokenType());
        if (tmp == nullptr) {
            break;
        }
        auto *t = new TreeNode(TreeNode::NUL, tmp);
        t->setLevel(level + 1);
        root->push_back(t);
    }
    root->push_back(new TreeNode(TreeNode::NUL, popNextToken(Token::SEMI)));
    return root;
}

TreeNode *Parser::stepUntilToken(Token::TokenTag *types, int size, int level) {
    auto *root = new TreeNode(TreeNode::ERRORSTEP);
    root->setLevel(level);
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
        t->setLevel(level + 1);
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



