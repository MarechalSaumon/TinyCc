//
// Created by saumonbro on 2/21/25.
//

#include "Program.h"

#include <Logger.h>
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

bool Program::Returns()
{
    bool res = true;
    for (const auto &[name, func] : m_functions)
    {
        if (!func->Returns())
        {
            if (func->GetReturnType() == VOID)
            {
                Logger::Log("Function '" + func->GetPrototype() + "' is declared as void and should not return a value.", WARNING);
            }
            else
            {
                Logger::Log("Function '" + func->GetPrototype() + "' does not return a value on all execution path.", WARNING);
            }
            res = false;
        }
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
