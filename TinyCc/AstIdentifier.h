//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTIDENTIFIER_H
#define ASTIDENTIFIER_H
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "Ast.h"

class AstIdentifier final : public Ast
{
public:
    long Evaluate() override;
    explicit AstIdentifier(std::string value, ContextMap ctxt)
        : name(std::move(value))
        , context(std::move(ctxt))
    {}
    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;

private:
    std::string name;
    ContextMap context;
};

#endif // ASTIDENTIFIER_H
