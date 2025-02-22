//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTLITERAL_H
#define ASTLITERAL_H
#include "Ast.h"


class AstLiteral final : public Ast {
public:
    explicit AstLiteral(long value)
        : value(value) {
    }

    long Evaluate() override;
    std::string Compile(std::unordered_map<std::string, int>& offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;

private:
    long value;
};




#endif //ASTLITERAL_H
