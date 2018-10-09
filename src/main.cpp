#include <iostream>

#include "CMMLexer/Lexer.h"
#include "Structure/Token.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "need two params" << std::endl;
        return 1;
    }
    Lexer lexer = Lexer(argv[1]);
    Token token = lexer.getNext();
    while (token.getTag() != END) {
        std::cout << "<Line:" << token.getLine() << ", Column:" << token.getColumn() << ", " <<token.getTagName() << ", " <<token.getValue() << ">\n";
        token = lexer.getNext();
    }
    return 0;
}