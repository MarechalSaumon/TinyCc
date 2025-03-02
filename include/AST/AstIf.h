//
// Created by saumonbro on 2/22/25.
//

#ifndef ASTIF_H
#define ASTIF_H
#include "Ast.h"

class AstIf : public Ast
{
public:
    long Evaluate() override;

    AstIf(std::unique_ptr<Ast> condition, std::unique_ptr<Ast> block,
          std::unique_ptr<Ast> elseBlock);

    // AstFunctionCall(const std::shared_ptr<Function> &function, const
    // std::vector<std::unique_ptr<Ast>> &args);

    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    std::unique_ptr<Ast> Optimize() override;

private:
    std::unique_ptr<Ast> m_condition;
    std::unique_ptr<Ast> m_block;
    std::unique_ptr<Ast> m_elseBlock;
};

#endif // ASTIF_H
