//
// Created by saumonbro on 2/19/25.
//

#include <iostream>
#include <Logger.h>
#include <AST/AstBlock.h>
#include <AST/AstLiteral.h>
#include <memory>
#include <ostream>
#include <vector>
#include <AST/AstIf.h>

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
    Logger::Log("Optimizing Block", DEBUG);
    long last;
    for (size_t i = 0; i < m_statements.size(); i++)
    {
        if (m_statements[i]->GetValueType() == Literal)
        {
            last = m_statements[i]->Evaluate();
            m_statements.erase(m_statements.begin() + i);
        }
        auto cur = m_statements[i]->Optimize();
        if (cur)
        {
            m_statements[i] = std::move(cur);
        }
        //if (cur != nullptr)
       // {
        //    last = cur->Evaluate();
       //     m_statements.erase(m_statements.begin() + i);
       // }
    }
    if (m_statements.empty())
    {
        return std::make_unique<AstLiteral>(last);
    }
    return nullptr;
}

std::string AstBlock::Dump()
{
    Logger::Log("Dumping Block", DEBUG);
    std::string res = "";
    for (const auto &s : m_statements)
    {
        res += s->Dump();
        res += "\n";
    }
    return res;
}

bool AstBlock::Returns()
{
    for (const auto &s : m_statements)
    {
        if (s->Returns())
        {
            return true;
        }
    }
    return false;
}
