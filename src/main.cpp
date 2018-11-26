#include <iostream>
#include "CMMLexer/Lexer.h"
#include "Structure/Token.h"
#include "CMMParser/Parser.h"
#include "Structure/TreeNode.h"
#include <sstream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "need two params" << std::endl;
        return 1;
    }
    Parser parser = Parser(argv[1]);
    TreeNode *root = parser.analyse();
    if (root->getNext())
        root->getNext()->toString();
    delete root;
    return 0;
}