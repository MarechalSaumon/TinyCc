//
// Created by saumonbro on 2/21/25.
//

#ifndef PROGRAM_H
#define PROGRAM_H
#include <Function.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Program
{
public:
    std::string Compile();

    explicit Program(
        std::unordered_map<std::string, std::shared_ptr<Function>> functions);
    void Optimize() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Function>> m_functions;
};

#endif // PROGRAM_H
