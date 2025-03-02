cmake_minimum_required(VERSION 3.16.3)
project(TinyCc)

target_sources(TinyCc PRIVATE
        AstLiteral.cpp
        AstUnary.cpp
        AstIdentifier.cpp
        AstReference.cpp
        AstBinary.cpp
        AstBlock.cpp
        AstAssignment.cpp
        AstFunctionCall.cpp
        AstWhile.cpp
        AstIf.cpp
        AstReturn.cpp
)