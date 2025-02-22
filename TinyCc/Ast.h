//
// Created by saumonbro on 2/19/25.
//

#ifndef AST_H
#define AST_H
#include <memory>
#include <string>
#include <unordered_map>

class Ast {

public:
    virtual ~Ast()  = default;
    //virtual ~Ast() = default;
    virtual long Evaluate() = 0;
    virtual std::string Dump() = 0;

    virtual std::unique_ptr<Ast> Optimize()
    {
        return nullptr;
    }

    virtual std::string Compile(std::unordered_map<std::string, int>& offsets) = 0;
};

#endif //AST_H
