//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTBLOCK_H
#define ASTBLOCK_H
#include <memory>
#include <vector>

#include "Ast.h"

class AstBlock : public Ast
{
public:
    long Evaluate() override;
    explicit AstBlock(std::vector<std::unique_ptr<Ast>> statements);
    std::string Compile(ContextMap &offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;

private:
    std::vector<std::unique_ptr<Ast>> m_statements;
};

#endif // ASTBLOCK_H
