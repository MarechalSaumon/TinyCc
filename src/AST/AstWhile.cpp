//
// Created by saumonbro on 2/22/25.
//

#include <AST/AstWhile.h>
#include <Utils.h>

long AstWhile::Evaluate()
{
    return 0;
}

AstWhile::AstWhile(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> block)
    : m_condition(std::move(condition))
    , m_block(std::move(block))
{}

std::string AstWhile::Compile(ContextMap &offsets)
{
    std::string res;
    auto beginLabel = Utils::GetNewLocalLabel();
    auto label = Utils::GetNewLocalLabel();
    auto afterLabel = Utils::GetNewLocalLabel();
    res += beginLabel + ":\n";
    res += m_condition->Compile(offsets);
    res += "test %rax, %rax\n";
    res += "jz " + afterLabel + "\n";
    res += m_block->Compile(offsets);
    res += "jmp " + beginLabel + "\n";
    res += afterLabel + ":\n";
    return res;
}

std::string AstWhile::Dump()
{
    return "while (" + m_condition->Dump() + ")\n{" + m_block->Dump() + "}\n";
}

std::unique_ptr<Ast> AstWhile::Optimize()
{
    auto cnd = m_condition->Optimize();
    if (cnd != nullptr)
    {
        m_condition = std::move(cnd);
    }
    auto block = m_block->Optimize();
    if (block != nullptr)
    {
        m_block = std::move(block);
    }
    return nullptr;
}
