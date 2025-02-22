//
// Created by saumonbro on 2/21/25.
//


#ifndef ASTFUNCTION_H
#define ASTFUNCTION_H
#include <map>

#include "Ast.h"
#include "AstBlock.h"
#include "AstLiteral.h"


class Function {
public:
    long Call(std::unordered_map<std::string, std::unique_ptr<AstLiteral>> context);

    explicit Function(std::string  name, std::unique_ptr<AstBlock> body, const std::vector<std::string>& args,
    std::shared_ptr<std::map<std::string, std::unique_ptr<Ast>>> context
    );

    std::string Compile() const;
    std::string Dump();
    void Optimize();

    std::string GetName() {
        return m_name;
    }

private:
    std::string m_name;
    std::unique_ptr<Ast> m_body;
    std::vector<std::string> m_args;

    std::shared_ptr<std::map<std::string, std::unique_ptr<Ast>>> m_context;
};



#endif //ASTFUNCTION_H
