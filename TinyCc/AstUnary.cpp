//
// Created by saumonbro on 2/19/25.
//

#include "AstUnary.h"

#include "AstLiteral.h"


AstUnary::AstUnary(std::unique_ptr<Ast> operand, TokenType operation) : m_operation(operation),
                                                                        m_operand(std::move(operand)) {
}

long AstUnary::Evaluate() {
    return m_operation == TOKEN_MINUS ? -m_operand->Evaluate() : m_operand->Evaluate();
}

std::string AstUnary::Compile(std::unordered_map<std::string, int>& offsets)
{
    if (m_operation == TOKEN_MINUS)
    {
        std::string res = m_operand->Compile(offsets) ;
        return "not %rax\n";
    }
    return m_operand->Compile(offsets);
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
