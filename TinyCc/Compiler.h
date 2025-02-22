//
// Created by saumonbro on 2/19/25.
//

#ifndef COMPILER_H
#define COMPILER_H
#include <memory>
#include <string>
#include <unordered_map>

#include "Ast.h"
#include "Program.h"


class Compiler {
public:
    explicit Compiler(const std::string& path);

    static int GenerateExecutable(const std::string &filepath);

    int Compile(const std::string& out = "out.S");
private:

    std::unordered_map<std::string, int> offsets;
    std::string path;
    std::unique_ptr<Ast> root;
    std::unique_ptr<Program> m_program;
};



#endif //COMPILER_H
