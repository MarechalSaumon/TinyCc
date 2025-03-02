//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTBINARY_H
#define ASTBINARY_H
#include <Lexer/Token.hpp>
#include <memory>

#include "Ast.h"

class AstBinary : public Ast
{
public:
    long Evaluate() override;

    AstBinary(std::unique_ptr<Ast> left, std::unique_ptr<Ast> right,
              TokenType operation);

    std::string
    CompileWithRvalue(std::unordered_map<std::string, int> &offsets);

    std::string Compile(ContextMap &offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;

private:
    std::unique_ptr<Ast> m_left;
    std::unique_ptr<Ast> m_right;
    TokenType op;
};

#endif // ASTBINARY_H
