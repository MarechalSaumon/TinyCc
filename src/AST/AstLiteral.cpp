//
// Created by saumonbro on 2/19/25.
//

#include <Logger.h>
#include <AST/AstLiteral.h>

#include "Utils.h"

ValueType AstLiteral::GetValueType()
{
    return Literal;
}

long AstLiteral::Evaluate()
{
    return value;
}

std::string AstLiteral::Compile([[maybe_unused]] ContextMap &offsets)
{
    return Utils::MoveLiteralToRax(value);
}

std::unique_ptr<Ast> AstLiteral::Optimize()
{
    Logger::Log("Optimizing Literal", DEBUG);
    return std::make_unique<AstLiteral>(value);
}

std::string AstLiteral::Dump()
{
    Logger::Log("Dumping Literal", DEBUG);
    return std::to_string(value);
}

VariableType AstLiteral::UnderlyingType()
{
    return INTEGER;
}
