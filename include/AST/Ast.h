//
// Created by saumonbro on 2/19/25.
//

#ifndef AST_H
#define AST_H
#include <Enum.h>
#include <map>
#include <memory>
#include <string>

class Variable;

enum ValueType
{
    Literal,
    Lvalue,
};

class Ast;

using ContextMap =
    std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>>;

class Ast
{
public:
    virtual ~Ast() = default;
    inline virtual ValueType GetValueType()
    {
        return Lvalue;
    }

    virtual VariableType UnderlyingType()
    {
        return NONE;
    }

    // virtual ~Ast() = default;
    virtual long Evaluate() = 0;
    virtual std::string Dump() = 0;

    virtual std::unique_ptr<Ast> Optimize()
    {
        return nullptr;
    }

    virtual std::string Compile(ContextMap &offsets) = 0;
};

#endif // AST_H
