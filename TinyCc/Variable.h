//
// Created by saumonbro on 2/23/25.
//

#ifndef VARIABLE_H
#define VARIABLE_H
#include <string>
#include <utility>

#include "Ast.h"
#include "AstLiteral.h"
#include "Utils.h"

class Function;

enum VariableType
{
    INTEGER,
    STRING,
};

class Variable
{
public:
    Variable(const int offset, VariableType m_type, std::string m_name,
             std::unique_ptr<Ast> ref = std::make_unique<AstLiteral>(0))
        : m_offset(offset)
        , m_type(m_type)
        , m_name(std::move(m_name))
        , m_referenced(std::move(ref))
    {}

    void SetAst(std::unique_ptr<Ast> ref)
    {
        m_referenced = std::move(ref);
    }

    [[nodiscard]] std::string GetAssemblyAlias() const
    {
        if (m_offset > 0)
        {
            return std::to_string(-m_offset) + "(" + Utils::StackPtr() + ")";
        }
        return "";
        // return args_reg[-m_offset]; // weirdo
    }

    [[nodiscard]] bool IsOnTheStack() const
    {
        return m_offset > 0;
    }

    void SetOffset(const int offset)
    {
        m_offset = offset;
    }

    [[nodiscard]] int GetOffset() const
    {
        return m_offset;
    }

private:
    int m_offset;
    VariableType m_type;
    std::string m_name;
    std::unique_ptr<Ast> m_referenced;
};

#endif // VARIABLE_H
