//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTASSIGNMENT_H
#define ASTASSIGNMENT_H
#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "Ast.h"

class AstAssignment final : public Ast
{
public:
    long Evaluate() override;
    explicit AstAssignment(const std::string &left, std::unique_ptr<Ast> right,
                           ContextMap context);
    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    std::unique_ptr<Ast> Optimize() override;

private:
    std::string m_left{};
    std::unique_ptr<Ast> m_right{};
    ContextMap m_context;
};

#endif // ASTASSIGNMENT_H
