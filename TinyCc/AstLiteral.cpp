//
// Created by saumonbro on 2/19/25.
//

#include "AstLiteral.h"

#include "Utils.h"


long AstLiteral::Evaluate() {
    return value;
}

std::string AstLiteral::Compile(std::unordered_map<std::string, int>& offsets)
{
    return Utils::MoveLiteralToRax(value);
}

std::unique_ptr<Ast> AstLiteral::Optimize()
{
    return std::make_unique<AstLiteral>(value);
}

std::string AstLiteral::Dump()
{
    return std::to_string(value);
}
