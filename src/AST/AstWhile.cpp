//
// Created by saumonbro on 2/22/25.
//

#include <Logger.h>
#include <AST/AstWhile.h>
#include <Utils.h>
#include <AST/AstLiteral.h>

long AstWhile::Evaluate()
{
    return 0;
}

AstWhile::AstWhile(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> block)
    : m_condition(std::move(condition))
      , m_block(std::move(block))
{
}

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
    Logger::Log("Dumping While", DEBUG);
    return "while (" + m_condition->Dump() + ")\n{" + m_block->Dump() + "}\n";
}

std::unique_ptr<Ast> AstWhile::Optimize()
{
    Logger::Log("Optimizing While", DEBUG);
    auto cnd = m_condition->Optimize();
    auto block = m_block->Optimize();
    if (block != nullptr)
    {
        m_block = std::move(block);
    }

    if (cnd != nullptr)
    {
        m_condition = std::move(cnd);
        long res = m_condition->Evaluate();

        if (!res)
        {
            return std::make_unique<AstLiteral>(0);
        }
    }

    return nullptr;
}

bool AstWhile::Returns()
{
    // If the condition is a literal that returns false, the block will have been completly removed
    // during optimization, so we know the condition returns true
    if (m_condition->GetValueType() == Literal)
    {
        return m_block->Returns();
    }
    return false;
}
