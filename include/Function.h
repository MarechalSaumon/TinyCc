//
// Created by saumonbro on 2/21/25.
//

#ifndef ASTFUNCTION_H
#define ASTFUNCTION_H
#include <map>
#include <vector>
#include <AST/Ast.h>

class Function
{
public:
    virtual ~Function() = default;

    explicit Function(std::string name, const std::vector<std::string> &args,
                      std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> context, std::shared_ptr<Context> global_context, bool isStatic = false, VariableType returnType = INTEGER);


    [[nodiscard]] std::string Compile();
    [[nodiscard]] std::string Dump() const;

    [[nodiscard]] std::string virtual GetPrototype() const;

    void Optimize();

    [[nodiscard]] bool Returns() const;

    std::string GetName()
    {
        return m_name;
    }

    [[nodiscard]] unsigned long virtual GetArgc() const
    {
        return static_cast<unsigned long>(m_args.size());
    }

    void AddVariable(const std::string& name, std::shared_ptr<Variable> variable) const;

    void SetBody(std::unique_ptr<Ast> body)
    {
        m_body = std::move(body);
    }

    [[nodiscard]] bool IsStatic() const
    {
        return m_isStatic;
    }

    [[nodiscard]] VariableType GetReturnType() const
    {
        return m_returnType;
    }

private:
    std::string m_name;
    std::unique_ptr<Ast> m_body;
    std::vector<std::string> m_args;
    bool m_isStatic;
    std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> m_context;
    VariableType m_returnType;
    std::shared_ptr<Context> global_context;
};

#endif // ASTFUNCTION_H
