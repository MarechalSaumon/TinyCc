//
// Created by saumonbro on 2/19/25.
//

#include <AST/AstIdentifier.h>
#include <Utils.h>
#include <Variables/Variable.h>

long AstIdentifier::Evaluate()
{
    return 0;
    // return (*context)[name]->Evaluate();
}

std::string AstIdentifier::Compile(ContextMap &offsets)
{
    return Utils::MoveToRax((*offsets)[name]->GetAssemblyAlias());
}

std::string AstIdentifier::Dump()
{
    return this->name;
}

VariableType AstIdentifier::UnderlyingType()
{
    return context->at(name)->GetType();
}
