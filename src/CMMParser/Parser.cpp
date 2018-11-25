//
// Created by chasi on 2018/11/11.
//

#include "Parser.h"

Parser::Parser(std::string path) {
    lexer = new Lexer(std::move(path));
    nextToken = nullptr;
    nextNextToken = nullptr;
    errorLists = std::list<ParserException *>();
}

Parser::~Parser() {
    delete lexer;
    lexer = nullptr;
    for (auto i:errorLists) {
        delete i;
        i = nullptr;
    }
    errorLists.clear();
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
        getNextToken();
    }
    while (getNextNextTokenType() == Token::MUL_NOTE || getNextNextTokenType() == Token::LINE_NOTE) {
        nextNextToken = lexer->getNext();
    }
    if (getNextTokenType() == Token::END){
        throw ParserException("In file's end",nextToken->getLine(),ParserException::FILE_END);
    }
}


Token::TokenTag Parser::getNextTokenType() {
    return nextToken->getTag();
}

Token::TokenTag Parser::getNextNextTokenType() {
    return nextNextToken->getTag();
}

Token *Parser::popNextToken(Token::TokenTag type) {
    if (type == getNextTokenType()) {
        Token *tmp = nextToken;
        getNextToken();
        return tmp;
    } else {
        errorLists.push_back(
                new ParserException("expected:" + Token::tokenTagMap.find(type)->second, nextToken->getLine(),
                                    nextToken->getErrorMessage()));
        Token *tmp = new Token(type, nextToken->getValue(), nextToken->getLine(), nextToken->getColumn(),
                               nextToken->getErrorMessage());
        //getNextToken();
        return tmp;
    }

}

Token *Parser::popNextToken(Token::TokenTag *types, int size) {
    for (int i = 0; i < size; i++) {
        if (types[i] == getNextTokenType()) {
            Token *tmp = nextToken;
            getNextToken();
            return tmp;
        }
    }
    errorLists.push_back(new ParserException("need expected tokens", nextToken->getLine(),
                                             nextToken->getErrorMessage()));
    Token *tmp = new Token(types[0], nextToken->getValue(), nextToken->getLine(), nextToken->getColumn(),
                           nextToken->getErrorMessage());
    // getNextToken();
    return tmp;
}

TreeNode *Parser::parseCharacter(Token::TokenTag type, int level) {
    auto *node = new TreeNode(NUL, popNextToken(type));
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
        default:
            throw ParserException("unexpected token:" + nextToken->getTagName(), nextToken->getLine(),
                                  nextToken->getErrorMessage());
    }
}

TreeNode *Parser::parseIfStmt(int level) {
    auto *node = new TreeNode(IF_STMT, popNextToken(Token::IF));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA, level + 1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA, level + 1));
    node->push_back(parseStmt(level + 1));
    if (getNextTokenType() == Token::ELSE) {
        node->push_back(parseCharacter(Token::ELSE, level + 1));
        node->push_back(parseStmt(level + 1));
    }
    return node;
}

TreeNode *Parser::parseWhileStmt(int level) {
    auto *node = new TreeNode(WHILE_STMT, popNextToken(Token::WHILE));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
    node->push_back(parseStmt(level + 1));
    return node;
}

TreeNode *Parser::parseForStmt(int level) {
    auto *node = new TreeNode(FOR_STMT, popNextToken(Token::FOR));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
    node->push_back(parseAssignStmt(level + 1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::SEMI,level+1));
    node->push_back(parseAssignStmt(level + 1, true));
    node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
    node->push_back(parseStmt(level + 1));
    return node;
}

TreeNode *Parser::parseReadStmt(int level) {
    auto *node = new TreeNode(READ_STMT, popNextToken(Token::READ));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
    node->push_back(parseVariableName(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
    node->push_back(parseCharacter(Token::SEMI,level+1));
    return node;
}

TreeNode *Parser::parseWriteStmt(int level) {
    auto *node = new TreeNode(WRITE_STMT, popNextToken(Token::WRITE));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
    node->push_back(parseCharacter(Token::SEMI,level+1));
    return node;
}

TreeNode *Parser::parseDeclareStmt(int level, bool isParseFun) {
    Token::TokenTag type[] = {Token::INT, Token::REAL};
    auto *node = new TreeNode(DECLARE_STMT, popNextToken(type, 2));
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
            node->push_back(parseCharacter(Token::ASSIGN,level+1));
            if (varNode->getChild().empty()) {
                node->push_back(parseExp(level + 1));
            }
        }
        if (isParseFun) {

        } else {
            node->push_back(parseCharacter(Token::SEMI,level+1));
        }

    }
    return node;
}

TreeNode *Parser::parseAssignStmt(int level, bool isParseFor) {
    auto *node = new TreeNode(ASSIGN_STMT);
    node->setLevel(level);
    node->push_back(parseVariableName(level + 1));
    node->push_back(parseCharacter(Token::ASSIGN,level+1));
    node->push_back(parseExp(level + 1));
    if (isParseFor) {

    } else {
        node->push_back(parseCharacter(Token::SEMI,level+1));
    }
    return node;
}

TreeNode *Parser::parseStmtBlock(int level) {
    auto *node = new TreeNode(BLOCK_STMT);
    node->setLevel(level);
    TreeNode *header = node;
    TreeNode *temp;
    node->push_back(parseCharacter(Token::LEFT_BOUNDER,level+1));
    while (getNextTokenType() != Token::RIGHT_BOUNDER) {
        temp = parseStmt(level + 1);
        node->push_back(temp);
        node = temp;
    }
    node->push_back(parseCharacter(Token::RIGHT_BOUNDER,level+1));
    return header;
}


TreeNode *Parser::parseExp(int level) {
    auto *node = new TreeNode(EXP);
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
    auto *node = new TreeNode(EXP);
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
    auto *node = new TreeNode(EXP);
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
    auto *node = new TreeNode(FACTOR);
    node->setLevel(level);
    switch (getNextTokenType()) {
        case Token::NUM:
            node->push_back(parseLiteral(level + 1));
            break;
        case Token::LEFT_BRA:
            node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
            node = parseExp(level + 1);
            node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
            break;
        case Token::MINUS:
            node->push_back(parseCharacter(Token::MINUS,level+1));
            node->push_back(parseTerm(level + 1));
            break;
        case Token::PLUS:
            node->push_back(parseCharacter(Token::PLUS,level+1));
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
    auto *node = new TreeNode(LITERAL, popNextToken(Token::NUM));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseLogicOp(int level) {
    Token::TokenTag types[6] = {Token::EQL, Token::NOT_EQL, Token::GRT, Token::GRT_EQL, Token::LES, Token::LES_EQL};
    auto *node = new TreeNode(OP, popNextToken(types, 6));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseAddtiveOp(int level) {
    Token::TokenTag types[2] = {Token::PLUS, Token::MINUS};
    auto *node = new TreeNode(OP, popNextToken(types, 2));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseMutiplyOp(int level) {
    Token::TokenTag types[2] = {Token::MUL, Token::DIV};
    auto *node = new TreeNode(OP, popNextToken(types, 2));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseVariableName(int level) {
    auto *node = new TreeNode(VAR, popNextToken(Token::IDENTIFIER));
    node->setLevel(level);
    return node;
}


TreeNode *Parser::parseFunctionDeclare(int level) {
    auto *node = new TreeNode(FUNDECLARE, popNextToken(Token::IDENTIFIER));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
    Token::TokenTag types[2] = {Token::INT, Token::REAL};
    TreeNode *header, *temp;
    if (checkNextTokenType(types, 2)) {
        header = parseDeclareStmt(level + 1, true);
        node->push_back(header);
        while (getNextTokenType() != Token::RIGHT_BRA) {
            node->push_back(parseCharacter(Token::COMMA,level+1));
            if (!checkNextTokenType(types, 2)) {
                throw ParserException("need variable", nextToken->getLine(), nextToken->getErrorMessage());
            }
            temp = parseDeclareStmt(level + 1, true);
            header->setNext(temp);
            header = temp;
        }
    }
    node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
    return node;
}


TreeNode *Parser::parseFunctionCall(int level, bool isStmt) {
    auto *node = new TreeNode(FUNCALL, popNextToken(Token::IDENTIFIER));
    node->setLevel(level);
    node->push_back(parseCharacter(Token::LEFT_BRA,level+1));
    TreeNode *header, *temp;
    if (getNextTokenType() != Token::RIGHT_BRA) {
        header = parseExp(level + 1);
        node->push_back(header);
        while (getNextTokenType() != Token::RIGHT_BRA) {
            node->push_back(parseCharacter(Token::COMMA,level+1));
            temp = parseExp(level + 1);
            header->setNext(temp);
            header = temp;
        }
    }
    node->push_back(parseCharacter(Token::RIGHT_BRA,level+1));
    if (isStmt) {
        node->push_back(parseCharacter(Token::SEMI,level+1));
    }
    return node;
}


TreeNode *Parser::parseReturn(int level) {
    auto *node = new TreeNode(RETURN_STMT, popNextToken(Token::RETURN));
    node->setLevel(level);
    node->push_back(parseExp(level + 1));
    node->push_back(parseCharacter(Token::SEMI,level+1));
    return node;
}

TreeNode *Parser::analyse() {
    auto *root = new TreeNode(NUL);
    TreeNode *head = root;
    getNextToken();
    try {
        while (nextToken->getTag() != Token::END) {
            TreeNode *node = parseStmt(0);
            root->setNext(node);
            root = node;
        }
    } catch (ParserException &pe) {
        std::cout << pe.message() << '\n';
    }
    for (auto i : errorLists) {
        std::cout << i->message() << '\n';
    }
    return head;
}



