//
// Created by saumonbro on 3/4/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include <AST/Ast.h>
#include <set>

class Context {
    public:

    private:
        std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> offsets;
        std::set<std::string> ro_data;
};



#endif //CONTEXT_H
