//
// Created by chasi on 2018/11/11.
//

#ifndef CMMCOMPILER_PARSER_H
#define CMMCOMPILER_PARSER_H

#include <string>
#include "CMMLexer/Lexer.h"
#include "Structure/TreeNode.h"
#include "Structure/Token.h"
#include "ParserException.h"
#include <list>

class Parser {
private:
    Lexer *lexer;
    Token *nextToken;
    Token *nextNextToken;
    std::list<ParserException *> errorLists;

    Token *popNextToken(Token::TokenTag type);

    Token *popNextToken(Token::TokenTag *types, int size);

    void getNextToken();

    bool checkNextTokenType(Token::TokenTag types[], int size);

    Token::TokenTag getNextTokenType();

    Token::TokenTag getNextNextTokenType();

    TreeNode *parseStmt(int level);

    TreeNode *parseIfStmt(int level);

    TreeNode *parseWhileStmt(int level);

    TreeNode *parseForStmt(int level);

    TreeNode *parseReadStmt(int level);

    TreeNode *parseWriteStmt(int level);

    TreeNode *parseDeclareStmt(int level, bool isParseFun = false);

    TreeNode *parseAssignStmt(int level, bool isParseFor = false);

    TreeNode *parseStmtBlock(int level);

    TreeNode *parseExp(int level);

    TreeNode *parseAddtiveExp(int level);

    TreeNode *parseTerm(int level);

    TreeNode *parseFactor(int level);

    TreeNode *parseLiteral(int level);

    TreeNode *parseLogicOp(int level);

    TreeNode *parseAddtiveOp(int level);

    TreeNode *parseMutiplyOp(int level);

    TreeNode *parseVariableName(int level);

    TreeNode *parseFunctionDeclare(int level);

    TreeNode *parseFunctionCall(int level, bool isStmt = true);

    TreeNode *parseReturn(int level);

    TreeNode *parseCharacter(Token::TokenTag type, int level);


public:
    explicit Parser(std::string path);

    TreeNode *analyse();

    ~Parser();
};


#endif //CMMCOMPILER_PARSER_H
