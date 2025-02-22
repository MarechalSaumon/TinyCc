//
// Created by saumonbro on 2/21/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

#include "AstBlock.h"


class Utils {
public:

    static std::string StackPtr() {
        return "%rbp";
    }

    static std::string MoveToRax(int offset)
    {
        return std::string("mov ") + std::to_string(-offset) + "(" + StackPtr() + "), %rax\n";
    }

    static std::string MoveFromRax(int offset)
    {
        return std::string("mov %rax, ") + std::to_string(-offset) + "(" + StackPtr() + ")\n";
    }


    static std::string MoveLiteralToRax(long value)
    {
        return std::string("mov $") + std::to_string(value) + ", %rax\n";
    }

    static std::string BuildPrologue(const std::unordered_map<std::string, int>& offsets) {
        std::string res;
        res += "push %rbp\n";
        res += "movq %rsp, %rbp\n";
        res += "sub $" + std::to_string((offsets.size() + 1) * 8  ) + ", %rsp\n\n\n";
        return res;
    }

    static std::string BuildEpilogue(const std::unordered_map<std::string, int>& offsets) {
        std::string res;
        res += "\n\n\nadd $" + std::to_string( (offsets.size() + 1) * 8) + ", %rsp\n";
        res += "pop %rbp\n";
        res += "ret\n";
        return res;
    }
};



#endif //UTILS_H
