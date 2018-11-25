//
// Created by WF on 2018/9/7.
//

// 保留字：if、else、while、read、write、int、bool、real
// 运算符：+、-、*、/、=、==、<>、<、>
// 分隔符：(、)、{、}、[、]、,、;
// 注释符：//、/*、*/

#include "Lexer.h"


std::map<std::string, Token::TokenTag> Lexer::keywordMap = {{"if",     Token::IF},
                                                            {"else",   Token::ELSE},
                                                            {"while",  Token::WHILE},
                                                            {"for",    Token::FOR},
                                                            {"read",   Token::READ},
                                                            {"write",  Token::WRITE},
                                                            {"int",    Token::INT},
                                                            {"real",   Token::REAL},
                                                            {"break",  Token::BREAK},
                                                            {"switch", Token::SWITCH},
                                                            {"case",   Token::CASE},
                                                            {"return", Token::RETURN}};
typedef enum {
    START,
    IGNORE,
    DONE
} StateType;

/**
 * 初始化Lexer
 * @param filePath 文件路径
 */
Lexer::Lexer(std::string filePath) {
    reader = new Reader(std::move(filePath));
    regex = new Regex();
}

/**
 * 获取下一个token
 * @return Token *
 */
Token *Lexer::getNext() {
    StateType stateType = START;
    Token::TokenTag tokenTag = Token::UNDEFINED;
    int currentColumn = column;
    int currentLine = line;
    std::string value = std::string();
    std::string errorMessage = std::string();
    bool isError = false;

    // 获取下一个token
    while (stateType != DONE) {

        //读取下一个字符
        currentChar = reader->getNextChar();
        column += 1;


        // 忽略行注释
        if (stateType == IGNORE && tokenTag == Token::LINE_NOTE) {
            if (currentChar == '\n') {
                line += 1;
                column = 1;
                stateType = DONE;
            }
            continue;
        }

        // 忽略段注释
        if (stateType == IGNORE && tokenTag == Token::MUL_NOTE) {
            if (currentChar == END_CHAR) {
                errorMessage = "Unterminated /* comment";
                isError = true;
                stateType = DONE;
            }
            if (currentChar != '*') {
                if (currentChar == '\n') {
                    line += 1;
                    column = 1;
                }
                continue;
            }
            currentChar = reader->getNextChar();
            column += 1;
            if (currentChar == '/') {
                stateType = DONE;
            }
            continue;
        }


        // 如果到了文件末
        if (currentChar == END_CHAR) {
            stateType = DONE;
            if (tokenTag == Token::UNDEFINED) {
                tokenTag = Token::END;
            } else {
                reader->setBack();
            }
            continue;
        }

        // 忽略空格和\t
        if (currentChar == ' ' || currentChar == '\t') {
            if (tokenTag != Token::UNDEFINED) {
                stateType = DONE;
            } else {
                currentColumn += 1;
            }
            continue;
        }

        // 处理换行符
        if (currentChar == '\n') {
            line += 1;
            column = 1;
            if (tokenTag != Token::UNDEFINED) {
                stateType = DONE;
            } else {
                currentLine = line;
                currentColumn = column;
            }
            continue;
        }

        // 处理token第一个字符
        if (tokenTag == Token::UNDEFINED) {
            if (currentChar == '[') {
                tokenTag = Token::LEFT_INDEX;
            }
            if (currentChar == ']') {
                tokenTag = Token::RIGHT_INDEX;
            }
            if (currentChar == '{') {
                tokenTag = Token::LEFT_BOUNDER;
            }
            if (currentChar == '}') {
                tokenTag = Token::RIGHT_BOUNDER;
            }
            if (currentChar == '(') {
                tokenTag = Token::LEFT_BRA;
            }
            if (currentChar == ')') {
                tokenTag = Token::RIGHT_BRA;
            }
            if (currentChar == ',') {
                tokenTag = Token::COMMA;
            }
            if (currentChar == ';') {
                tokenTag = Token::SEMI;
            }
            if (currentChar == '+') {
                tokenTag = Token::PLUS;
            }
            if (currentChar == '-') {
                tokenTag = Token::MINUS;
            }
            if (currentChar == '*') {
                tokenTag = Token::MUL;
            }
            if (currentChar == '/') {
                tokenTag = Token::DIV;
            }
            if (currentChar == '%') {
                tokenTag = Token::MOD;
            }
            if (currentChar == '=') {
                tokenTag = Token::ASSIGN;
            }
            if (currentChar == '<') {
                tokenTag = Token::LES;
            }
            if (currentChar == '>') {
                tokenTag = Token::GRT;
            }
            if (currentChar == '\'') {
                tokenTag = Token::SIN_QUE;
            }
            if (currentChar == '\"') {
                tokenTag = Token::DOU_QUE;
            }
            if (isdigit(currentChar) || currentChar == '.') {
                tokenTag = Token::NUM;
            }
            if (isalpha(currentChar)) {
                tokenTag = Token::IDENTIFIER;
            }
            value.append(1, currentChar);
        }
            // 处理双字符运算符
        else if (tokenTag == Token::ASSIGN) {
            if (currentChar == '=') {
                tokenTag = Token::EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        } else if (tokenTag == Token::LES) {
            if (currentChar == '=') {
                tokenTag = Token::LES_EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else if (currentChar == '>') {
                tokenTag = Token::NOT_EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        } else if (tokenTag == Token::GRT) {
            if (currentChar == '=') {
                tokenTag = Token::GRT_EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        } else if (tokenTag == Token::DIV) {
            if (currentChar == '/') {
                tokenTag = Token::LINE_NOTE;
                stateType = IGNORE;
                value.append(1, currentChar);
            } else if (currentChar == '*') {
                tokenTag = Token::MUL_NOTE;
                stateType = IGNORE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        }
            // 处理数字
        else if (tokenTag == Token::NUM) {
            if (specialChar.find(currentChar) != -1) {
                stateType = DONE;
                reader->setBack();
            } else {
                value.append(1, currentChar);
                continue;
            }
        }
            // 处理标识符
        else if (tokenTag == Token::IDENTIFIER) {
            if (specialChar.find(currentChar) != -1) {
                stateType = DONE;
                reader->setBack();
            } else {
                value.append(1, currentChar);
            }
            continue;
        } else {
            stateType = DONE;
            reader->setBack();
        }


    }


    // 判断标识符是否是关键字，是否是正确的标识符格式
    if (tokenTag == Token::IDENTIFIER) {
        if (keywordMap.find(value) != keywordMap.end()) {
            tokenTag = keywordMap.find(value)->second;
        }

        if (!regex->isIdentifier(value)) {
            errorMessage = "ILLEGAL_IDENTIFIER";
            isError = true;
        }
    }

    // 判断是否是正确的数字格式
    if (tokenTag == Token::NUM) {
        if (!regex->isNum(value)) {
            errorMessage = "ILLEGAL_NUMBER";
            isError = true;
        }
    }
    return new Token(tokenTag, value, currentLine, currentColumn, errorMessage, isError);

}

Lexer::~Lexer() {
    delete reader;
    reader = nullptr;
    delete regex;
    regex = nullptr;
}

