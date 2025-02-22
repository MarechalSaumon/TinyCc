//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTREFERENCE_H
#define ASTREFERENCE_H
#include <memory>
#include <string>
#include <unordered_map>

#include "Ast.h"

class AstReference final : public Ast {
public:
    long Evaluate() override;
    std::string Compile(std::unordered_map<std::string, int>& offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;

private:
    std::string name;
    std::shared_ptr<std::unordered_map<std::string, std::unique_ptr<Ast>>> context;
};


#endif //ASTREFERENCE_H
