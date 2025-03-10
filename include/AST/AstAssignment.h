//
// Created by saumonbro on 2/19/25.
//

#ifndef ASTASSIGNMENT_H
#define ASTASSIGNMENT_H
#include <map>
#include <memory>
#include <string>

#include <AST/Ast.h>

class AstAssignment final : public Ast
{
public:



    long Evaluate() override;
    explicit AstAssignment(const std::string &left, std::unique_ptr<Ast> right,
                            std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> context);
    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    std::unique_ptr<Ast> Optimize() override;

    VariableType UnderlyingType() override;


private:
    std::string m_left{};
    std::unique_ptr<Ast> m_right{};
     std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>>  m_context;
};

#endif // ASTASSIGNMENT_H
