//
// Created by saumonbro on 3/6/25.
//

#include <Utils.h>
#include <AST/AstLiteralString.h>


ValueType AstLiteralString::GetValueType()
{
    return Lvalue;
}

long AstLiteralString::Evaluate()
{
    return 0;
}

std::string AstLiteralString::Compile([[maybe_unused]] ContextMap &offsets)
{
    return Utils::MoveLiteralToRax(value);
}


std::string AstLiteralString::Dump()
{
    Logger::Log("Dumping Literal string", DEBUG);
    return value;
}

VariableType AstLiteralString::UnderlyingType()
{
    return STRING;
}
