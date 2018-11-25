cmake_minimum_required(VERSION 3.12)
project(CMMCompiler)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-static")
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
include_directories(src)

add_executable(CMMCompiler src/main.cpp src/Structure/Token.cpp src/Structure/Token.h src/Utils/Reader.cpp src/Utils/Reader.h src/CMMLexer/Lexer.cpp src/CMMLexer/Lexer.h src/CMMLexer/Regex.h src/CMMLexer/Regex.cpp src/Utils/DFA.cpp src/Utils/DFA.h src/CMMParser/Parser.cpp src/CMMParser/Parser.h src/Structure/TreeNode.cpp src/Structure/TreeNode.h src/CMMParser/ParserException.cpp src/CMMParser/ParserException.h)

find_package(Threads REQUIRED)
target_link_libraries(CMMCompiler Threads::Threads)