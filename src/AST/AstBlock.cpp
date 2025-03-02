//
// Created by saumonbro on 2/19/25.
//

#include <AST/AstBlock.h>
#include <AST/AstLiteral.h>
#include <memory>
#include <vector>

long AstBlock::Evaluate()
{
    long res = 0;
    for (const auto &s : m_statements)
    {
        res = s->Evaluate();
    }
    return res;
}
AstBlock::AstBlock(std::vector<std::unique_ptr<Ast>> statements)
{
    m_statements = std::move(statements);
}

std::string AstBlock::Compile(ContextMap &offsets)
{
    std::string res = "";
    for (const auto &s : m_statements)
    {
        res += s->Compile(offsets);
    }
    return res;
}

std::unique_ptr<Ast> AstBlock::Optimize()
{
    long last;
    for (size_t i = 0; i < m_statements.size(); i++)
    {
        auto cur = m_statements[i]->Optimize();
        if (cur != nullptr)
        {
            last = cur->Evaluate();
            m_statements.erase(m_statements.begin() + i);
        }
    }
    if (m_statements.empty())
    {
        return std::make_unique<AstLiteral>(last);
    }
    return nullptr;
}

std::string AstBlock::Dump()
{
    std::string res = "";
    for (const auto &s : m_statements)
    {
        res += s->Dump();
        res += "\n";
    }
    return res;
}
