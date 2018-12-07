//
// Created by chasi on 2018/11/11.
//

#ifndef CMMCOMPILER_PARSER_H
#define CMMCOMPILER_PARSER_H

#include <string>
#include <list>
#include <stack>
#include <vector>

#include "Lexer/Lexer.h"
#include "Structure/TreeNode.h"
#include "Structure/Token.h"
#include "ParserException.h"

class Parser {
private:
    Lexer *lexer;
    Token *nextToken;
    Token *nextNextToken;
    std::list<Token *> tokens;
    std::list<ParserException *> errorLists;
    std::list<TreeNode *> unParsered;
    std::vector<TreeNode *> treeNodeVec;
    std::list<std::string> classList;

    Token *popNextToken(Token::TokenTag type);

    Token *popNextToken(Token::TokenTag *types, int size);

    void getNextToken();

    bool checkNextTokenType(Token::TokenTag types[], int size);

    Token::TokenTag getNextTokenType();

    Token::TokenTag getNextNextTokenType();

    TreeNode *parseStmt();

    TreeNode *parseClassDeclare();

    TreeNode *parseIfStmt();

    TreeNode *parseWhileStmt();

    TreeNode *parseForStmt();

    TreeNode *parseReadStmt();

    TreeNode *parseWriteStmt();

    TreeNode *parseNewStmt();

    TreeNode *parseDeclareStmt(bool isParseFun = false);

    TreeNode *parseAssignStmt(bool isParseFor = false);

    TreeNode *parseStmtBlock();

    TreeNode *parseExp();

    TreeNode *parseAddtiveExp();

    TreeNode *parseTerm();

    TreeNode *parseFactor();

    TreeNode *parseLiteral();

    TreeNode *parseLogicOp();

    TreeNode *parseAddtiveOp();

    TreeNode *parseMutiplyOp();

    TreeNode *parseVariableName();

    TreeNode *parseFunctionDeclare(bool isConstructor = false);

    TreeNode *parseFunctionCall(bool isStmt = true);

    TreeNode *parseReturn();

    TreeNode *parseCharacter(Token::TokenTag type);

    TreeNode *stepUntilTokenSEMI();

    TreeNode *stepUntilToken(Token::TokenTag types[], int size);


public:
    explicit Parser(std::string path);

    std::vector<TreeNode *> analyse();

    std::vector<TreeNode *> clearCharaterNode(std::vector<TreeNode *> &nodeVec);

    ~Parser();
};


#endif //CMMCOMPILER_PARSER_H
