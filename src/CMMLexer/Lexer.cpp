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

Lexer::Lexer(std::string filePath) {
    reader = new Reader(std::move(filePath));
    regex = new Regex();
    keywordSet.insert("if");
    keywordSet.insert("else");
    keywordSet.insert("while");
    keywordSet.insert("for");
    keywordSet.insert("read");
    keywordSet.insert("write");
    keywordSet.insert("int");
    keywordSet.insert("real");
}

Token Lexer::getNext() {
    StateType stateType = START;
    TokenTag tokenTag = UNDEFINED;
    int currentColumn = column;
    int currentLine = line;
    std::string value = std::string();
    while (stateType != DONE) {
        currentChar = reader->getNextChar();
        column += 1;
        if (currentChar == END_CHAR) {
            stateType = DONE;
            if (tokenTag == UNDEFINED) {
                tokenTag = END;
            } else {
                reader->setBack();
            }
            continue;
        }
        if (stateType == IGNORE && tokenTag == LINE_NOTE) {
            if (currentChar == '\n') {
                line += 1;
                column = 1;
                stateType = DONE;
            }
            continue;
        }

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
        if (currentChar == ' ' || currentChar == '\t') {
            if (tokenTag != UNDEFINED) {
                stateType = DONE;
            } else {
                currentColumn += 1;
            }
            continue;
        }

        if (currentChar == '\n') {
            if (tokenTag != UNDEFINED) {
                stateType = DONE;
            } else {
                line += 1;
                column = 1;
                currentLine = line;
                currentColumn = column;
            }
            continue;
        }

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
        } else if (tokenTag == ASSIGN) {
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
        } else if (tokenTag == NUM) {
            if (specialChar.find(currentChar) != -1) {
                stateType = DONE;
                reader->setBack();
            } else {
                value.append(1, currentChar);
                continue;
            }
        } else if (tokenTag == IDENTIFIER) {
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
    if (tokenTag == IDENTIFIER) {
        if(keywordSet.find(value) != keywordSet.end()){
            tokenTag = KEYWORD;
        }
    }
    return Token(tokenTag, value, currentLine, currentColumn);

}

Lexer::~Lexer() {
    delete reader;
    reader = nullptr;
    delete regex;
    regex = nullptr;
}

