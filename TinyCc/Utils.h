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

static unsigned CurrentLabel = 0;

class Utils
{
public:
    static std::string LogicalNot(const std::string &reg)
    {
        std::string res;
        res += "xor %rax, %rax\n";
        res += "test " + reg + ", " + reg + "\n";
        res += "sete %al\n";
        return res;
    }

    static std::string LogicalOr(const std::string &reg1,
                                 const std::string &reg2)
    {
        std::string res_false = GetNewLocalLabel();
        std::string res_true = GetNewLocalLabel();
        std::string res;
        res += "cmp $0," + reg1 + "\n";
        res += "je " + res_false + "\n";
        res += "cmp $0," + reg2 + "\n";
        res += "je " + res_false + "\n";
        res += "mov $1, %rax";
        res += "jmp " + res_true + "\n";
        res += res_false + ":\n";
        res += "xor %rax, %rax\n";
        res += res_true + ":\n";
        return res;
    }

    static std::string StackPtr()
    {
        return "%rbp";
    }

    static std::string MoveRegisterToStack(const std::string &reg, int offset)
    {
        return std::string("mov ") + reg + ", " + std::to_string(-offset) + "("
            + StackPtr() + ")\n";
    }

    static std::string MoveRegisterToRegister(const std::string &reg1,
                                              const std::string &reg2)
    {
        return std::string("mov ") + reg1 + ", " + reg2 + "\n";
    }

    static std::string MoveToRax(int offset)
    {
        return std::string("mov ") + std::to_string(-offset) + "(" + StackPtr()
            + "), %rax\n";
    }

    static std::string MoveFromRax(int offset)
    {
        return std::string("mov %rax, ") + std::to_string(-offset) + "("
            + StackPtr() + ")\n";
    }

    static std::string MoveLiteralToStack(const std::string &literal,
                                          int offset)
    {
        return std::string("movq " + literal + ", ") + std::to_string(-offset)
            + "(" + StackPtr() + ")\n";
    }

    static std::string MoveLiteralToRax(long value)
    {
        return std::string("mov $") + std::to_string(value) + ", %rax\n";
    }

    static std::string
    BuildPrologue(const std::unordered_map<std::string, int> &offsets)
    {
        std::string res;
        res += "push %rbp\n";
        res += "movq %rsp, %rbp\n";
        res += "sub $" + std::to_string((offsets.size()) * 8) + ", %rsp\n\n";
        return res;
    }

    static std::string
    BuildEpilogue(const std::string &func,
                  const std::unordered_map<std::string, int> &offsets)
    {
        std::string res;
        res += "\n.Lend" + func + ":\n";
        res += "add $" + std::to_string((offsets.size()) * 8) + ", %rsp\n";
        res += "pop %rbp\n";
        res += "ret\n\n\n";
        return res;
    }

    static std::string GetNewLocalLabel()
    {
        return ".L" + std::to_string(CurrentLabel++);
    }
};

#endif // UTILS_H
