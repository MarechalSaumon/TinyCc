//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTUNARY_H
#define ASTUNARY_H
#include <memory>

#include "Ast.h"
#include "Token.hpp"



class AstUnary final : public Ast {

public:
    AstUnary(std::unique_ptr<Ast> operand, TokenType operation);

    long Evaluate() override;
    std::string Compile(std::unordered_map<std::string, int>& offsets) override;
    std::unique_ptr<Ast> Optimize() override;
    std::string Dump() override;


private:
    std::unique_ptr<Ast> m_operand;
    TokenType m_operation;
};


#endif //ASTUNARY_H
