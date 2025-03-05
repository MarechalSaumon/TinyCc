//
// Created by saumonbro on 2/22/25.
//

#include <iostream>
#include <Logger.h>
#include <AST/AstReturn.h>

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
    Logger::Log("Dumping Return statement", DEBUG);
    return "return " + m_expression->Dump() + ";\n";
}

std::unique_ptr<Ast> AstReturn::Optimize()
{
    Logger::Log("Optimizing Return", DEBUG);

    auto newStatement = m_expression->Optimize();
    if (newStatement)
    {
        m_expression = std::move(newStatement);
    }
    return nullptr;
}

VariableType AstReturn::UnderlyingType()
{
    return m_expression->UnderlyingType();
}

bool AstReturn::Returns()
{
    return true;
}
