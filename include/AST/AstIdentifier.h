//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTIDENTIFIER_H
#define ASTIDENTIFIER_H
#include <map>
#include <memory>
#include <string>

#include <AST/Ast.h>

class AstIdentifier final : public Ast
{
public:

    long Evaluate() override;
    explicit AstIdentifier(std::string value,  std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>>  ctxt)
        : name(std::move(value))
        , context(std::move(ctxt))
    {}
    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    VariableType UnderlyingType() override;
    std::string GetName() const
    {
        return name;
    }

private:
    std::string name;
     std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> context;
};

#endif // ASTIDENTIFIER_H
