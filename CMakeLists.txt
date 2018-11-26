cmake_minimum_required(VERSION 3.12)
project(CMMCompiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-static")
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
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
        src/Lexer/SymbolTable.cpp src/Lexer/SymbolTable.h)

find_package(Threads REQUIRED)
target_link_libraries(CMMCompiler Threads::Threads)