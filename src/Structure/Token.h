//
// Created by WF on 2018/9/6.
//

#ifndef CMMCOMPILER_TOKEN_H
#define CMMCOMPILER_TOKEN_H

#include <string>

enum TokenTag {
    UNDEFINED,
    ENDFILE, ERROR, END,
    // identifier
    IDENTIFIER,
    // number
    NUM,
    // keyword
    IF, ELSE, WHILE, FOR, READ, WRITE, INT, REAL,
    BREAK, SWITCH, CASE, RETURN,
    // operator
    // +
    PLUS,
    // -
    MINUS,
    // *
    MUL,
    // /
    DIV,
    // %
    MOD,
    // = < <= > >= == <>
    ASSIGN, LES, LES_EQL, GRT, GRT_EQL, EQL, NOT_EQL,
    // separator
    // ( )
    LEFT_BRA, RIGHT_BRA,
    // [ ]
    LEFT_INDEX, RIGHT_INDEX,
    // { }
    LEFT_BOUNDER, RIGHT_BOUNDER,
    // . , ; ' "
    POINTER, COMMA, SEMI, SIN_QUE, DOU_QUE,
    // annotation
    // //
    LINE_NOTE,
    // /* */
    MUL_NOTE
};

class Token {
private:

    /* token类型 */
    TokenTag tag;

    /* token内容 */
    std::string value;

    /* token所在行 */
    int line;

    /* token所在列 */
    int column;

public:
    /**
     * 构造token
     * @param t token类型
     * @param v token内容
     * @param l token所在行
     * @param c token所在列
     */
    Token(TokenTag t, std::string v, int l, int c);

    TokenTag getTag() const;

    const std::string &getValue() const;

    int getLine() const;

    int getColumn() const;
};


#endif //CMMCOMPILER_TOKEN_H
