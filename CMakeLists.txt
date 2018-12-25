cmake_minimum_required(VERSION 3.12)
project(CMMCompiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-static")
include_directories(src)

add_executable(CMMCompiler
        src/main.cpp
        src/Structure/Token.cpp src/Structure/Token.h
        src/Utils/Reader.cpp src/Utils/Reader.h
        src/Lexer/Lexer.cpp src/Lexer/Lexer.h
        src/Lexer/Regex.h src/Lexer/Regex.cpp
        src/Parser/Parser.cpp src/Parser/Parser.h
        src/Structure/TreeNode.cpp src/Structure/TreeNode.h
        src/Parser/ParserException.cpp src/Parser/ParserException.h
        src/Structure/SymbolTable.cpp src/Structure/SymbolTable.h
        src/Structure/AbstractSymbol.cpp src/Structure/AbstractSymbol.h
        src/Semantic/Semantic.cpp src/Semantic/Semantic.h
        src/Structure/Quaternary.cpp src/Structure/Quaternary.h
        src/Semantic/SemanticException.cpp src/Semantic/SemanticException.h
        src/Structure/Value.cpp src/Structure/Value.h
        src/Structure/FunctionSymbol.cpp src/Structure/FunctionSymbol.h
        src/Structure/VarSymbol.cpp src/Structure/VarSymbol.h
        src/Structure/ClassSymbol.cpp src/Structure/ClassSymbol.h
        src/Utils/Common.cpp src/Utils/Common.h
        src/Executor/Executor.cpp src/Executor/Executor.h
        src/Executor/ExecutorException.cpp src/Executor/ExecutorException.h src/Structure/Object.cpp src/Structure/Object.h)

find_package(Threads REQUIRED)
target_link_libraries(CMMCompiler Threads::Threads)