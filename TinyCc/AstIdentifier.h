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


class AstIdentifier final : public Ast {
public:
    long Evaluate() override;
    explicit AstIdentifier(std::string  value, std::shared_ptr<std::map<std::string, std::unique_ptr<Ast>>>  ctxt)
            : name(std::move(value)), context(std::move(ctxt)) {
    }
    std::string Compile(std::unordered_map<std::string, int>& offsets) override;
    std::string Dump() override;

private:
    std::string name;
    std::shared_ptr<std::map<std::string, std::unique_ptr<Ast>>> context;
};


#endif //ASTIDENTIFIER_H
