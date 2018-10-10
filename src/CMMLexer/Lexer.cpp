//
// Created by WF on 2018/9/7.
//

// 保留字：if、else、while、read、write、int、bool、real
// 运算符：+、-、*、/、=、==、<>、<、>
// 分隔符：(、)、{、}、[、]、,、;
// 注释符：//、/*、*/

typedef enum {
    START,
    IGNORE,
    DONE
} StateType;

#include "Lexer.h"

/**
 * 初始化Lexer
 * @param filePath 文件路径
 */
Lexer::Lexer(std::string filePath) {
    reader = new Reader(std::move(filePath));
    regex = new Regex();

    // 初始化关键字set
    keywordSet.insert("if");
    keywordSet.insert("else");
    keywordSet.insert("while");
    keywordSet.insert("for");
    keywordSet.insert("read");
    keywordSet.insert("write");
    keywordSet.insert("int");
    keywordSet.insert("real");
}

/**
 * 获取下一个token
 * @return Token
 */
Token Lexer::getNext() {
    StateType stateType = START;
    TokenTag tokenTag = UNDEFINED;
    int currentColumn = column;
    int currentLine = line;
    std::string value = std::string();
    std::string errorMessage = std::string();

    // 获取下一个token
    while (stateType != DONE) {

        //读取下一个字符
        currentChar = reader->getNextChar();
        column += 1;

        // 如果到了文件末
        if (currentChar == END_CHAR) {
            stateType = DONE;
            if (tokenTag == UNDEFINED) {
                tokenTag = END;
            } else {
                reader->setBack();
            }
            continue;
        }

        // 忽略行注释
        if (stateType == IGNORE && tokenTag == LINE_NOTE) {
            if (currentChar == '\n') {
                line += 1;
                column = 1;
                stateType = DONE;
            }
            continue;
        }

        // 忽略段注释
        if (stateType == IGNORE && tokenTag == MUL_NOTE) {
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


        // 忽略空格和\t
        if (currentChar == ' ' || currentChar == '\t') {
            if (tokenTag != UNDEFINED) {
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
            if (tokenTag != UNDEFINED) {
                stateType = DONE;
            } else {
                currentLine = line;
                currentColumn = column;
            }
            continue;
        }

        // 处理token第一个字符
        if (tokenTag == UNDEFINED) {
            if (currentChar == '[') {
                tokenTag = LEFT_INDEX;
            }
            if (currentChar == ']') {
                tokenTag = RIGHT_INDEX;
            }
            if (currentChar == '{') {
                tokenTag = LEFT_BOUNDER;
            }
            if (currentChar == '}') {
                tokenTag = RIGHT_BOUNDER;
            }
            if (currentChar == '(') {
                tokenTag = LEFT_BRA;
            }
            if (currentChar == ')') {
                tokenTag = RIGHT_BRA;
            }
            if (currentChar == ',') {
                tokenTag = COMMA;
            }
            if (currentChar == ';') {
                tokenTag = SEMI;
            }
            if (currentChar == '+') {
                tokenTag = PLUS;
            }
            if (currentChar == '-') {
                tokenTag = MINUS;
            }
            if (currentChar == '*') {
                tokenTag = MUL;
            }
            if (currentChar == '/') {
                tokenTag = DIV;
            }
            if (currentChar == '%') {
                tokenTag = MOD;
            }
            if (currentChar == '=') {
                tokenTag = ASSIGN;
            }
            if (currentChar == '<') {
                tokenTag = LES;
            }
            if (currentChar == '>') {
                tokenTag = GRT;
            }
            if (currentChar == '\'') {
                tokenTag = SIN_QUE;
            }
            if (currentChar == '\"') {
                tokenTag = DOU_QUE;
            }
            if ((currentChar <= '9' && currentChar >= '0') || currentChar == '.') {
                tokenTag = NUM;
            }
            if ((currentChar <= 'z' && currentChar >= 'a') || (currentChar <= 'Z' && currentChar >= 'A')) {
                tokenTag = IDENTIFIER;
            }
            value.append(1, currentChar);
        }
            // 处理双字符运算符
        else if (tokenTag == ASSIGN) {
            if (currentChar == '=') {
                tokenTag = EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        } else if (tokenTag == LES) {
            if (currentChar == '=') {
                tokenTag = LES_EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else if (currentChar == '>') {
                tokenTag = NOT_EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        } else if (tokenTag == GRT) {
            if (currentChar == '=') {
                tokenTag = GRT_EQL;
                stateType = DONE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        } else if (tokenTag == DIV) {
            if (currentChar == '/') {
                tokenTag = LINE_NOTE;
                stateType = IGNORE;
                value.append(1, currentChar);
            } else if (currentChar == '*') {
                tokenTag = MUL_NOTE;
                stateType = IGNORE;
                value.append(1, currentChar);
            } else {
                stateType = DONE;
                reader->setBack();
            }
        }
            // 处理数字
        else if (tokenTag == NUM) {
            if (specialChar.find(currentChar) != -1) {
                stateType = DONE;
                reader->setBack();
            } else {
                value.append(1, currentChar);
                continue;
            }
        }
            // 处理标识符
        else if (tokenTag == IDENTIFIER) {
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
    if (tokenTag == IDENTIFIER) {
        if (keywordSet.find(value) != keywordSet.end()) {
            tokenTag = KEYWORD;
        }

        if (!regex->isIdentifier(value)) {
            errorMessage = "ILLEGAL_IDENTIFIER";
            tokenTag = ERROR;
        }
    }

    // 判断是否是正确的数字格式
    if (tokenTag == NUM) {
        if (!regex->isNum(value)) {
            errorMessage = "ILLEGAL_NUMBER";
            tokenTag = ERROR;
        }
    }
    return Token(tokenTag, value, currentLine, currentColumn, errorMessage);

}

Lexer::~Lexer() {
    delete reader;
    reader = nullptr;
    delete regex;
    regex = nullptr;
}

