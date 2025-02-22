//
// Created by saumonbro on 2/19/25.
//

#include "AstBinary.h"

#include <ios>
#include <iostream>
#include <ostream>
#include <bits/ios_base.h>

#include "AstLiteral.h"

long AstBinary::Evaluate() {
    switch (op) {
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
            throw std::invalid_argument("Invalid operation " + Token::tokenToString(op));
    }
}

AstBinary::AstBinary(std::unique_ptr<Ast> left, std::unique_ptr<Ast> right, TokenType operation) : op(operation), m_left(std::move(left)), m_right(std::move(right)) {
}

std::string CompileAdd() {

}

std::string AstBinary::Compile(std::unordered_map<std::string, int>& offsets)
{
    std::string res = "push %rbx\n"; // save rbx
    res += m_left->Compile(offsets); // rax contains result
    res += "mov %rax, %rbx\n"; // Store the result of left in rbx
    res += m_right->Compile(offsets);
    if (op == TOKEN_PLUS) {
        res += "add %rax, %rbx\n"; // Left =  rbx, right = rax
    }
    if (op == TOKEN_MINUS) {
        res += "sub %rax, %rbx\n";
    }
    if (op == TOKEN_MUL) {
        res += "imul %rax, %rbx\n";
    }
    if (op == TOKEN_DIV) {
        res += "xchg %rax, %rbx\n";
        res += "cqto\n";
        res += "idivq %rbx\n";
        res += "xchg %rax, %rbx\n";
    }
    if (op == TOKEN_AND) {
        res += "and %rax, %rbx\n";
    }
    if (op == TOKEN_OR) {
        res += "or %rax, %rbx\n";
    }
    res += "mov %rbx, %rax\n";
    res += "pop %rbx\n";
    return res;
}

std::unique_ptr<Ast> AstBinary::Optimize()
{
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

    if (!leftOpt || !rightOpt) {
        return nullptr;
    }
    return std::make_unique<AstLiteral>(Evaluate());
}

std::string AstBinary::Dump()
{
    return m_left->Dump() + " " + Token::opToString(op) + " " + m_right->Dump();
}
