cmake_minimum_required(VERSION 3.30)
project(TinyCc)

set(CMAKE_CXX_STANDARD 17)

add_executable(TinyCc main.cpp
        Token.cpp
        Token.hpp
        Lexer.cpp
        Lexer.h
        Parser.cpp
        Parser.h
        Ast.h
        AstLiteral.cpp
        AstLiteral.h
        AstUnary.cpp
        AstUnary.h
        AstIdentifier.cpp
        AstIdentifier.h
        AstReference.cpp
        AstReference.h
        AstBinary.cpp
        AstBinary.h
        AstBlock.cpp
        AstBlock.h
        AstAssignment.cpp
        AstAssignment.h
        Compiler.cpp
        Compiler.h
        Utils.cpp
        Utils.h
        Function.cpp
        Function.h
        AstFunctionCall.cpp
        AstFunctionCall.h
        Program.cpp
        Program.h)
