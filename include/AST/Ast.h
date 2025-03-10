//
// Created by saumonbro on 2/19/25.
//

#ifndef AST_H
#define AST_H
#include <Enum.h>
#include <Logger.h>
#include <memory>
#include <string>
class Variable;
class Context;
using ContextMap = Context;

enum ValueType
{
    Literal,
    Lvalue,
};

class Ast;
//std::shared_ptr<std::map<std::string, std::unique_ptr<Variable>>>;

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
        Logger::Log("Default optimize", DEBUG);
        return nullptr;
    }

    virtual std::string Compile(ContextMap &offsets) = 0;
    virtual bool Returns()
    {
        return false;
    }
};

#endif // AST_H
