//
// Created by saumonbro on 2/21/25.
//

#ifndef ASTFUNCTIONCALL_H
#define ASTFUNCTIONCALL_H
#include "Ast.h"
#include "AstBlock.h"
#include "Function.h"


class AstFunctionCall : public Ast {
public:
    long Evaluate() override;

    AstFunctionCall(const std::shared_ptr<Function> &function, const std::vector<Ast> &args);

    std::string Compile(std::unordered_map<std::string, int>& offsets) override;
    std::string Dump() override;
    std::unique_ptr<Ast> Optimize() override;
private:
    std::shared_ptr<Function> m_function;
    std::vector<Ast> m_args;
};



#endif //ASTFUNCTIONCALL_H
