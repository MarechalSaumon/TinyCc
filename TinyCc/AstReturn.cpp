//
// Created by saumonbro on 2/22/25.
//

#include "AstReturn.h"

long AstReturn::Evaluate()
{
    return m_expression->Evaluate();
}

AstReturn::AstReturn(const std::string &func, std::unique_ptr<Ast> expression)
    : m_function(func)
{
    m_expression = std::move(expression);
}

std::string AstReturn::Compile(ContextMap &offsets)
{
    return m_expression->Compile(offsets) + "jmp .Lend" + m_function + "\n";
}

std::string AstReturn::Dump()
{
    return "return " + m_expression->Dump() + ";\n";
}

std::unique_ptr<Ast> AstReturn::Optimize()
{
    return Ast::Optimize();
}
