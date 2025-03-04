//
// Created by saumonbro on 2/23/25.
//

#ifndef VARIABLE_H
#define VARIABLE_H
#include <AST/Ast.h>
#include <AST/AstLiteral.h>
#include <Utils.h>
#include <string>
#include <Enum.h>


class Variable
{
public:
    virtual ~Variable() = default;

    Variable(int offset, std::string m_name,
             std::unique_ptr<Ast> ref = std::make_unique<AstLiteral>(0));

    void SetAst(std::unique_ptr<Ast> ref);

    [[nodiscard]] std::string GetAssemblyAlias() const;
    void SetOffset(int offset);

    /*[[nodiscard]] bool IsOnTheStack() const
    {
        return m_offset > 0;
    }*/
    std::string OperatorError(const std::string& op, const Variable& other) const
    {
        return ("Invalid operator: "+op+" over " + VariableTypeToString(GetType()) + " and " + VariableTypeToString(other.GetType()));
    }

    virtual std::shared_ptr<Variable> operator+(const Variable &other) const
    {
        throw std::runtime_error(OperatorError("+",other));
    }
    virtual std::shared_ptr<Variable> operator-(const Variable &other) const
    {
        throw std::runtime_error(OperatorError("-",other));
    }
    virtual std::shared_ptr<Variable> operator*(const Variable &other) const
    {
        throw std::runtime_error(OperatorError("*",other));
    }
    virtual std::shared_ptr<Variable> operator/(const Variable &other) const
    {
        throw std::runtime_error(OperatorError("/",other));
    }
    virtual VariableType GetType() const = 0;


private:
    int m_offset;
    std::string m_name;
    std::unique_ptr<Ast> m_referenced;
};


std::unique_ptr<Variable> make_variable(int offset, std::string m_name, VariableType type);

#endif // VARIABLE_H
