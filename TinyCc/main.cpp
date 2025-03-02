#include <fstream>
#include <iostream>

#include "Compiler.h"
#include "Lexer.h"
#include "Parser.h"

int main()
{
    auto lang = "C++";
    std::cout << "Hello and welcome to " << lang << "!\n";

    Compiler compiler("/home/saumonbro/Desktop/CodeForFun/C++/TinyCC2/TinyCc/"
                      "TinyCc/test.txt");

    compiler.Compile();

    return 0;
}
