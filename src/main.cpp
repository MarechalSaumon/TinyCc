#include <Compiler.h>
#include <Lexer/Lexer.h>
#include <Parser/Parser.h>
#include <fstream>
#include <iostream>

int main()
{
    auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    Compiler compiler(
        "/home/saumonbro/Desktop/CodeForFun/C++/TinyCC2/TinyCc/test.txt");

    compiler.Compile();

    return 0;
}
