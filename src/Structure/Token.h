//
// Created by chasi on 2018/9/6.
//

#ifndef CMMCOMPILER_TOKEN_H
#define CMMCOMPILER_TOKEN_H

#include <string>
#include <map>


class Token {
public:
    enum TokenTag {
        // undefined

        UNDEFINED,
        // file end

        END,
        // identifier

        IDENTIFIER,
        // number

        NUM,
        INT_VALUE,
        REAL_VALUE,
        // char

        CHAR_VALUE,
        // keyword

        IF,
        ELSE,
        WHILE,
        FOR,
        READ,
        WRITE,
        INT,
        REAL,
        CHAR,
        RETURN,
        NEW,
        CLASS,
        CLASS_NAME,
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

        ASSIGN,
        LES,
        LES_EQL,
        GRT,
        GRT_EQL,
        EQL,
        NOT_EQL,
        // separator
        // ( )

        LEFT_BRA,
        RIGHT_BRA,
        // [ ]

        LEFT_INDEX,
        RIGHT_INDEX,
        // { }

        LEFT_BOUNDER,
        RIGHT_BOUNDER,
        // . , ; ' "

        POINTER,
        COMMA,
        SEMI,
        SIN_QUE,
        DOU_QUE,
        // annotation
        // //

        LINE_NOTE,
        // /* */

        MUL_NOTE,

        LOGIC_EXP,
        ADDTIVE_EXP,
        TERM_EXP
    };

    Token();

    /**
     * 构造token
     * @param t token类型
     * @param v token内容
     * @param l token所在行
     * @param c token所在列
     * @param error 错误信息
     */
    Token(TokenTag t, std::string v, int l, int c, std::string error, bool isErr = false);

    TokenTag getTag() const;

    std::string getTagName() const;

    const std::string &getValue() const;

    int getLine() const;

    int getColumn() const;

    const std::string &getErrorMessage() const;

    static std::map<TokenTag, std::string> tokenTagMap;

    bool isIsError() const;

    Token *clone();

private:
    /* token类型 */
    TokenTag tag;

    /* token内容 */
    std::string value;

    /* token所在行 */
    int line;

    /* token所在列 */
    int column;

    /* token错误信息*/
    std::string errorMessage;

    bool isError;

};


#endif //CMMCOMPILER_TOKEN_H
