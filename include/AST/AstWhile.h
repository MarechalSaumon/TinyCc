//
// Created by saumonbro on 2/22/25.
//

#ifndef ASTWHILE_H
#define ASTWHILE_H
#include "Ast.h"

class AstWhile : public Ast
{
public:
    long Evaluate() override;

    AstWhile(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> block);

    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    std::unique_ptr<Ast> Optimize() override;

private:
    std::unique_ptr<Ast> m_condition;
    std::unique_ptr<Ast> m_block;
};

#endif // ASTWHILE_H
