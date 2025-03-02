//
// Created by saumonbro on 2/21/25.
//

#include "Function.h"

#include <list>
#include <utility>

#include "Utils.h"

long Function::Call(
    std::unordered_map<std::string, std::unique_ptr<AstLiteral>> context)
{
    (void)context;
    return 0;
}

Function::Function(std::string name, const std::vector<std::string> &args,
                   ContextMap context, bool isStatic)
    : m_name(std::move(name))
    , m_args(args)
    , m_isStatic(isStatic)
    , m_context(std::move(context))
{}

// Args not in offset
std::string Function::Compile()
{
    auto offsets = std::unordered_map<std::string, int>();
    int offset = 1;
    for (const auto &s : *m_context)
    {
        offsets[s.first] = offset * 8;
        s.second->SetOffset(offset * 8);
        offset++;
    }

    // compute offsets for each argument
    for (const auto &arg : m_args)
    {
        if (*m_context->find(arg) == *m_context->end())
        {
            offsets[arg] = offset * 8;
            offset++;
        }
    }

    std::string res;
    res += m_name + ":\n\n";
    res += Utils::BuildPrologue(offsets);

    // move args on the stack

    for (size_t i = 0; i < m_args.size(); i++)
    {
        res += Utils::MoveRegisterToStack(args_reg[i], offsets[m_args[i]]);
    }

    res += m_body->Compile(m_context);
    res += "\n";
    res += Utils::BuildEpilogue(m_name, offsets);
    return res;
}

std::string Function::Dump()
{
    std::string res;
    res += m_name + ":\n\n";
    res += m_body->Dump();
    res += "\n";
    return res;
}

void Function::Optimize()
{
    std::unique_ptr<Ast> cur = m_body->Optimize();
    if (cur == nullptr)
    {
        return;
    }
    m_body = std::move(cur);
}
