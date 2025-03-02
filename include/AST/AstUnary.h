//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTUNARY_H
#define ASTUNARY_H
#include <Lexer/Token.hpp>
#include <memory>

#include "Ast.h"

class AstUnary final : public Ast
{
public:
    AstUnary(std::unique_ptr<Ast> operand, TokenType operation);

    long Evaluate() override;
    std::string Compile(ContextMap &offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;

private:
    std::unique_ptr<Ast> m_operand;
    TokenType m_operation;
};

#endif // ASTUNARY_H
