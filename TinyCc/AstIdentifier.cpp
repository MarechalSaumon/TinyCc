//
// Created by saumonbro on 2/19/25.
//

#include "AstIdentifier.h"

#include "Utils.h"


long AstIdentifier::Evaluate() {
    return (*context)[name]->Evaluate();
}

std::string AstIdentifier::Compile(std::unordered_map<std::string, int>& offsets)
{
    return Utils::MoveToRax(offsets[name]);
}

std::string AstIdentifier::Dump()
{
    return this->name;
}
