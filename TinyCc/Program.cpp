//
// Created by saumonbro on 2/21/25.
//

#include "Program.h"

#include <utility>

std::string Program::Compile()
{
    std::string res;
    res += ".section .text\n";
    for (const auto &func : m_functions)
    {
        if (!func.second->IsStatic())
        {
            res += "    .global " + func.second->GetName() + "\n";
        }
    }
    res += "\n";
    for (const auto &[_, func] : m_functions)
    {
        res += func->Compile();
    }

    return res;
}

Program::Program(
    std::unordered_map<std::string, std::shared_ptr<Function>> functions)
    : m_functions(std::move(functions))
{}

void Program::Optimize() const
{
    for (const auto &[_, func] : m_functions)
    {
        func->Optimize();
    }
}
