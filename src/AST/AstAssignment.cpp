//
// Created by saumonbro on 2/19/25.
//

#include <AST/AstAssignment.h>
#include <AST/AstLiteral.h>
#include <Utils.h>
#include <Variables/Variable.h>
#include <iostream>
#include <Logger.h>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

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
    //int index = (*offsets)[m_left]->GetOffset();

    const std::string& asmName = (*offsets)[m_left]->GetAssemblyAlias();

    if (m_right->GetValueType() == Literal) // literal
    {
        if (m_right->UnderlyingType() == STRING) // Write it in .rodata
        {

        }
        const std::string val = "$" + std::to_string(m_right->Evaluate());
        return Utils::MoveLiteralToStack(val, asmName);
    }


    std::string res = m_right->Compile(offsets);
    return res + Utils::MoveFromRax(asmName);
}

std::string AstAssignment::Dump()
{
    Logger::Log("Dumping Assignment", DEBUG);
    return "set " + m_left + " = " + m_right->Dump();
}

std::unique_ptr<Ast> AstAssignment::Optimize()
{
    Logger::Log("Optimizing Assignment", DEBUG);
    // std::cout << "WOOOO Optimize " << std::endl;
    auto right = m_right->Optimize();
    if (right != nullptr)
    {
        // std::cout << "Right not null feur ! " << std::endl;
        m_right = std::move(right);
    }
    return nullptr;
}

VariableType AstAssignment::UnderlyingType(){
    return m_right->UnderlyingType();
}
