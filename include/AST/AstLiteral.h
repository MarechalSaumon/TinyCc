//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTLITERAL_H
#define ASTLITERAL_H
#include <variant>

#include "Ast.h"

class AstLiteral final : public Ast
{
public:

    explicit AstLiteral(long value)
        : value(value)
    {}

    ValueType GetValueType() override;

    long Evaluate() override;
    std::string Compile(ContextMap &offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;
    VariableType UnderlyingType() override;

private:
    long value;
};

#endif // ASTLITERAL_H
