cmake_minimum_required(VERSION 3.12)
project(CMMCompiler)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(CMMCompiler src/main.cpp src/Structure/Token.cpp src/Structure/Token.h src/Utils/Reader.cpp src/Utils/Reader.h src/CMMLexer/Lexer.cpp src/CMMLexer/Lexer.h src/CMMLexer/Regex.h src/CMMLexer/Regex.cpp src/CMMLexer/DFA.cpp src/CMMLexer/DFA.h)