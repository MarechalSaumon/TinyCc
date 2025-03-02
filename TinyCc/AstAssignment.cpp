//
// Created by saumonbro on 2/19/25.
//

#include "AstAssignment.h"

#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

#include "AstLiteral.h"
#include "Utils.h"
#include "Variable.h"

long AstAssignment::Evaluate()
{
    long val = m_right->Evaluate();
    (*m_context)[m_left]->SetAst(std::make_unique<AstLiteral>(val));
    return val;
}

AstAssignment::AstAssignment(const std::string &left,
                             std::unique_ptr<Ast> right, ContextMap context)
{
    m_context = std::move(context);
    m_left = left;
    m_right = std::move(right);
}

std::string AstAssignment::Compile(ContextMap &offsets)
{
    int index = (*offsets)[m_left]->GetOffset();
    ;

    if (m_right->Type() == Literal) // literal
    {
        const std::string val = "$" + std::to_string(m_right->Evaluate());
        return Utils::MoveLiteralToStack(val, index);
    }

    std::string res = m_right->Compile(offsets);
    return res + Utils::MoveFromRax(index);
}

std::string AstAssignment::Dump()
{
    return "set " + m_left + " = " + m_right->Dump();
}

std::unique_ptr<Ast> AstAssignment::Optimize()
{
    std::cout << "WOOOO Optimize " << std::endl;
    auto right = m_right->Optimize();
    if (right != nullptr)
    {
        std::cout << "Right not null feur ! " << std::endl;
        m_right = std::move(right);
    }
    return nullptr;
}
