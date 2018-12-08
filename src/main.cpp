#include <iostream>
#include "Lexer/Lexer.h"
#include "Structure/Token.h"
#include "Parser/Parser.h"
#include "Structure/TreeNode.h"
#include <fstream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "need two params" << std::endl;
        return 1;
    }
    Parser parser = Parser(argv[1]);
    auto treeNodeVec = parser.analyse();
    treeNodeVec = parser.clearCharaterNode(treeNodeVec);
    std::string parserFile = std::string(argv[1]) + ".parser";
    std::ofstream file(parserFile);
    for (auto i: treeNodeVec) {
        file << i->toString();
    }
    file.close();
    return 0;
}