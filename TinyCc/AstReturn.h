//
// Created by saumonbro on 2/22/25.
//

#ifndef ASTRETURN_H
#define ASTRETURN_H
#include "Ast.h"

class AstReturn : public Ast
{
public:
    long Evaluate() override;

    explicit AstReturn(const std::string &func,
                       std::unique_ptr<Ast> expression);

    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    std::unique_ptr<Ast> Optimize() override;

private:
    std::unique_ptr<Ast> m_expression;
    std::string m_function;
};

#endif // ASTRETURN_H
