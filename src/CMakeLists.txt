cmake_minimum_required(VERSION 3.16.3)
project(TinyCc)

add_subdirectory("AST")
add_subdirectory("Lexer")
add_subdirectory("Parser")

target_sources(TinyCc PRIVATE Compiler.cpp Function.cpp main.cpp Program.cpp Utils.cpp Variable.cpp)
