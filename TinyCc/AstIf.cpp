//
// Created by saumonbro on 2/22/25.
//

#include "AstIf.h"

#include "Utils.h"

long AstIf::Evaluate()
{
    long res = m_condition->Evaluate();
    if (res == 0)
    {
        return m_block->Evaluate();
    }
    if (m_elseBlock)
    {
        return m_elseBlock->Evaluate();
    }
    return 0;
}

AstIf::AstIf(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> block,
             std::unique_ptr<Ast> elseBlock)
    : m_condition(std::move(condition))
    , m_block(std::move(block))
    , m_elseBlock(std::move(elseBlock))
{}

std::string AstIf::Compile(ContextMap &offsets)
{
    std::string res;

    // No else case :
    auto label = Utils::GetNewLocalLabel();
    auto afterLabel = Utils::GetNewLocalLabel();
    auto elseLabel = Utils::GetNewLocalLabel();

    res += m_condition->Compile(offsets);
    res += "test %rax, %rax\n";
    res += "jz " + (m_elseBlock ? elseLabel : afterLabel) + "\n";
    res += label + ":\n";
    res += m_block->Compile(offsets);

    if (m_elseBlock != nullptr)
    {
        res += "jmp " + afterLabel + "\n";
        res += elseLabel + ":\n";
        res += m_elseBlock->Compile(offsets);
    }

    res += afterLabel + ":\n";
    return res;
}

std::string AstIf::Dump()
{
    return "";
}

std::unique_ptr<Ast> AstIf::Optimize()
{
    auto res = m_condition->Optimize();
    auto block = m_block->Optimize();
    std::unique_ptr<Ast> elseBlock = nullptr;
    if (m_elseBlock)
    {
        elseBlock = m_elseBlock->Optimize();
    }
    if (block != nullptr)
    {
        m_block = std::move(block);
    }

    if (elseBlock != nullptr)
    {
        m_elseBlock = std::move(elseBlock);
    }

    if (res != nullptr)
    {
        m_condition = std::move(res);
        auto val = m_condition->Evaluate();
        if (val == 0) // remove if
        {
            if (m_elseBlock)
            {
                return std::move(m_elseBlock);
            }
        }
        else
        {
            return std::move(m_block);
        }
    }
    return nullptr;
}
