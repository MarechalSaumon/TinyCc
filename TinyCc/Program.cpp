//
// Created by saumonbro on 2/21/25.
//

#include "Program.h"

#include <utility>

std::string Program::Compile()
{
    std::string res;
    res += ".global facto\n";
    res += ".text\n\n";
    for (const auto& s : m_functions)
    {
        res += s.second->Compile();
    }

    return res;
}

Program::Program(std::unordered_map<std::string, std::shared_ptr<Function>> functions) : m_functions(std::move(functions)) {
}

void Program::Optimize() const {
    for (const auto & s : m_functions) {
        s.second->Optimize();
    }
}
