#include <iostream>
#include "Lexer/Lexer.h"
#include "Structure/Token.h"
#include "Parser/Parser.h"
#include "Structure/TreeNode.h"
#include "Semantic/Semantic.h"
#include "Executor/Executor.h"
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

    std::string QuaFile = std::string(argv[1]) + ".Qua";
    std::ofstream quaFile(QuaFile);
    Semantic semantic = Semantic();
    std::string error;
    auto v = semantic.generateCode(treeNodeVec, error);
    _dataSize line = 0;
    for (auto &i: v) {
        quaFile << line++ << " " << i.toString();
    }
    quaFile.close();
    Executor executor(semantic.programEntry, v);
    executor.execute();
    return 0;
}