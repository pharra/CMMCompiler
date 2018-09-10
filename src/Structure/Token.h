//
// Created by WF on 2018/9/6.
//

#ifndef CMMCOMPILER_TOKEN_H
#define CMMCOMPILER_TOKEN_H

#include <string>

enum tokenTag {
    NUM, KEYWORD, IDENTIFIER, OPERATOR, ANNOTATION, END
};

class Token {
private:

    /* token类型 */
    tokenTag tag;

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
    Token(tokenTag t, std::string v, int l, int c);

    tokenTag getTag() const;

    const std::string &getValue() const;

    int getLine() const;

    int getColumn() const;
};


#endif //CMMCOMPILER_TOKEN_H
