//
// Created by saumonbro on 2/19/25.
//

#include <AST/AstBinary.h>
#include <AST/AstLiteral.h>
#include <Utils.h>
#include <bits/ios_base.h>
#include <ios>
#include <iostream>
#include <Logger.h>
#include <ostream>

long AstBinary::Evaluate()
{
    switch (op)
    {
    case TOKEN_PLUS:
        return m_left->Evaluate() + m_right->Evaluate();
    case TOKEN_MUL:
        return m_left->Evaluate() * m_right->Evaluate();
    case TOKEN_MINUS:
        return m_left->Evaluate() - m_right->Evaluate();
    case TOKEN_DIV:
        return m_left->Evaluate() / m_right->Evaluate();
    case TOKEN_MOD:
        return m_left->Evaluate() % m_right->Evaluate();
    case TOKEN_EQUAL:
        return m_left->Evaluate() == m_right->Evaluate();
    case TOKEN_GREATER:
        return m_left->Evaluate() > m_right->Evaluate();
    case TOKEN_LESS:
        return m_left->Evaluate() < m_right->Evaluate();
    case TOKEN_GREATER_EQUAL:
        return m_left->Evaluate() >= m_right->Evaluate();
    case TOKEN_LESS_EQUAL:
        return m_left->Evaluate() <= m_right->Evaluate();
    case TOKEN_NOTEQUAL:
        return m_left->Evaluate() != m_right->Evaluate();
    case TOKEN_AND:
        return m_left->Evaluate() && m_right->Evaluate();
    case TOKEN_OR:
        return m_left->Evaluate() || m_right->Evaluate();
    default:
        throw std::invalid_argument("Invalid operation "
                                    + Token::tokenToString(op));
    }
}

AstBinary::AstBinary(std::unique_ptr<Ast> left, std::unique_ptr<Ast> right,
                     const TokenType operation)
    : m_left(std::move(left))
    , m_right(std::move(right))
    , op(operation)
{}

static std::string GetOp(const TokenType operation)
{
    switch (operation)
    {
    case TOKEN_PLUS:
        return "add";
    case TOKEN_MUL:
        return "imul";
    case TOKEN_MINUS:
        return "sub";
    default:
        throw std::invalid_argument("Invalid operation "
                                    + Token::tokenToString(operation));
    }
}

std::string CompileSimpleOperation(const std::string &op,
                                   const std::string &left)
{
    return op + " " + left + ", %rbx\n";
}

std::string CompileSimpleOperationWithImmediateValue(const std::string &op,
                                                     long value)
{
    return op + "$" + std::to_string(value) + ", %rbx\n";
}

// %rax / %rbx
std::string CompileDivision()
{
    std::string res;
    res += "xchg %rax, %rbx\n";
    res += "cqto\n";
    res += "idivq %rbx\n";
    res += "xchg %rax, %rbx\n";
    return res;
}

// %rax == %rbx
std::string CompileEqual(const std::string &left)
{
    std::string res;
    res += "cmp " + left + ", %rbx\n";
    res += "sete %al\n";
    res += "movzbq %al, %rbx\n";
    return res;
}

std::string AstBinary::CompileWithString(ContextMap& offsets)
{
    std::string res = "push %rbx\n"; // save rbx

    res += m_left->Compile(offsets); // rax contains result
    res += "mov %rax, %rbx\n"; // Store the result of left in rbx
    res += m_right->Compile(offsets); // right in rax

    if (m_left->UnderlyingType() == STRING && m_right->UnderlyingType() == STRING)
    {
        if (op == TOKEN_PLUS) // Concat strings
        {
            res += "mov %rbx, %rdi\n";
            res += "mov %rax, %rsi\n";
            res += "call concat_str\n";
        }
        else
        {
            throw std::invalid_argument("Invalid operation ");
        }
    }
    else
    {
        throw std::invalid_argument("Invalid operation ");
    }

    return res + "pop %rbx\n";
}

// Result is stored in rbx, then moved to rax at the end of the function
std::string AstBinary::Compile(ContextMap &offsets)
{

    if (m_left->UnderlyingType() == STRING || m_right->UnderlyingType() == STRING)
    {
        return CompileWithString(offsets);
    }
    // This is good for numerals
    std::string res = "push %rbx\n"; // save rbx

    res += m_left->Compile(offsets); // rax contains result
    res += "mov %rax, %rbx\n"; // Store the result of left in rbx

    // Here, if right is a literal, lets not waste time
    std::string leftVal;
    if (m_right->GetValueType() == Literal && op != TOKEN_DIV)
    {
        leftVal = "$" + std::to_string(m_right->Evaluate());
    }
    else
    {
        leftVal = "%rax";
        res += m_right->Compile(offsets);
    }

    if (op == TOKEN_PLUS || op == TOKEN_MUL || op == TOKEN_MINUS)
    {
        res += CompileSimpleOperation(GetOp(op), leftVal);
        res += "mov %rbx, %rax\n";
    }

    if (op == TOKEN_DIV)
    {
        res += CompileDivision();
        res += "mov %rbx, %rax\n";
    }
    if (op == TOKEN_EQUAL || op == TOKEN_NOTEQUAL)
    {
        res += CompileEqual(leftVal);
        if (op == TOKEN_NOTEQUAL)
        { // logical not over result
            res += Utils::LogicalNot("%rbx");
        }
    }
    res += "pop %rbx\n";
    return res;
}

std::unique_ptr<Ast> AstBinary::Optimize()
{
    Logger::Log("Optimizing Binary", DEBUG);

    auto left = m_left->Optimize();
    auto right = m_right->Optimize();

    bool leftOpt = left != nullptr;
    bool rightOpt = right != nullptr;

    if (left != nullptr)
    {
        m_left = std::move(left);
    }
    if (right != nullptr)
    {
        m_right = std::move(right);
    }

    long res = 1;

    if (leftOpt)
        res = m_left->Evaluate();

    if (rightOpt)
        res = m_right->Evaluate();

    if (op == TOKEN_MUL && res == 0)
    {
        return std::make_unique<AstLiteral>(0);
    }

    if (!leftOpt || !rightOpt)
    {
        return nullptr;
    }
    return std::make_unique<AstLiteral>(Evaluate());
}

std::string AstBinary::Dump()
{
    Logger::Log("Dumping Binary", DEBUG);
    return m_left->Dump() + " " + Token::opToString(op) + " " + m_right->Dump();
}


VariableType AstBinary::UnderlyingType()
{
    if (m_left->UnderlyingType() == STRING || m_right->UnderlyingType() == STRING)
    {
        return VariableType::STRING;
    }
    return INTEGER;
}
