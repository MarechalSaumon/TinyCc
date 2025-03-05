//
// Created by saumonbro on 2/21/25.
//

#ifndef ASTFUNCTION_H
#define ASTFUNCTION_H
#include <AST/Ast.h>
#include <AST/AstBlock.h>
#include <AST/AstLiteral.h>
#include <Variables/Variable.h>
#include <map>

class Function
{
public:
    long
    Call(std::unordered_map<std::string, std::unique_ptr<AstLiteral>> context);

    explicit Function(std::string name, const std::vector<std::string> &args,
                      ContextMap context, bool isStatic = false, VariableType returnType = INTEGER);

    [[nodiscard]] std::string Compile();
    std::string Dump();

    std::string GetPrototype();

    void Optimize();

    bool Returns();

    std::string GetName()
    {
        return m_name;
    }

    [[nodiscard]] unsigned long GetArgc() const
    {
        return static_cast<unsigned long>(m_args.size());
    }

    void SetBody(std::unique_ptr<Ast> body)
    {
        m_body = std::move(body);
    }

    [[nodiscard]] bool IsStatic() const
    {
        return m_isStatic;
    }

    VariableType GetReturnType() const
    {
        return m_returnType;
    }

private:
    std::string m_name;
    std::unique_ptr<Ast> m_body;
    std::vector<std::string> m_args;
    bool m_isStatic;
    ContextMap m_context;
    VariableType m_returnType;
};

#endif // ASTFUNCTION_H
