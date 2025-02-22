//
// Created by saumonbro on 2/19/25.
//

#include "AstReference.h"

long AstReference::Evaluate()
{
    return 0;
}

std::string AstReference::Compile(std::unordered_map<std::string, int>& offsets)
{
    return "";
}

std::unique_ptr<Ast> AstReference::Optimize() {
    return Ast::Optimize();
}

std::string AstReference::Dump()
{
    return "&" + this->name;
}
