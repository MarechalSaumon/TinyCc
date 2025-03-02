//
// Created by saumonbro on 2/19/25.
//

#include <AST/AstLiteral.h>
#include <AST/AstUnary.h>
#include <Utils.h>

AstUnary::AstUnary(std::unique_ptr<Ast> operand, TokenType operation)
    : m_operand(std::move(operand))
    , m_operation(operation)
{}

long AstUnary::Evaluate()
{
    return m_operation == TOKEN_MINUS ? -m_operand->Evaluate()
                                      : m_operand->Evaluate();
}

std::string AstUnary::Compile(ContextMap &offsets)
{
    std::string res = m_operand->Compile(offsets);

    if (m_operation == TOKEN_NOT)
    {
        res += Utils::LogicalNot("%rax");
    }
    if (m_operation == TOKEN_MINUS)
    {
        res += "neg %rax\n";
    }
    return res;
}

std::unique_ptr<Ast> AstUnary::Optimize()
{
    auto newOp = m_operand->Optimize();
    if (newOp == nullptr) // Could not optimize
    {
        return nullptr;
    }
    m_operand = std::move(newOp);
    return std::make_unique<AstLiteral>(Evaluate());
}

std::string AstUnary::Dump()
{
    return Token::opToString(m_operation) + " " + m_operand->Dump();
}
