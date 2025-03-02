//
// Created by saumonbro on 2/21/25.
//

#include <Utils.h>

unsigned Utils::CurrentLabel = 0;

std::string Utils::LogicalNot(const std::string &reg)
{
    std::string res;
    res += "xor %rax, %rax\n";
    res += "test " + reg + ", " + reg + "\n";
    res += "sete %al\n";
    return res;
}

std::string Utils::LogicalOr(const std::string &reg1, const std::string &reg2)
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

std::string Utils::StackPtr()
{
    return "%rbp";
}

std::string Utils::MoveRegisterToStack(const std::string &reg, int offset)
{
    return std::string("mov ") + reg + ", " + std::to_string(-offset) + "("
        + StackPtr() + ")\n";
}

std::string Utils::MoveRegisterToRegister(const std::string &reg1,
                                          const std::string &reg2)
{
    return std::string("mov ") + reg1 + ", " + reg2 + "\n";
}

std::string Utils::MoveToRax(int offset)
{
    return std::string("mov ") + std::to_string(-offset) + "(" + StackPtr()
        + "), %rax\n";
}

std::string Utils::MoveFromRax(int offset)
{
    return std::string("mov %rax, ") + std::to_string(-offset) + "("
        + StackPtr() + ")\n";
}

std::string Utils::MoveLiteralToStack(const std::string &literal, int offset)
{
    return std::string("movq " + literal + ", ") + std::to_string(-offset) + "("
        + StackPtr() + ")\n";
}

std::string Utils::MoveLiteralToRax(long value)
{
    return std::string("mov $") + std::to_string(value) + ", %rax\n";
}

std::string
Utils::BuildPrologue(const std::unordered_map<std::string, int> &offsets)
{
    std::string res;
    res += "push %rbp\n";
    res += "movq %rsp, %rbp\n";
    res += "sub $" + std::to_string((offsets.size()) * 8) + ", %rsp\n\n";
    return res;
}

std::string
Utils::BuildEpilogue(const std::string &func,
                     const std::unordered_map<std::string, int> &offsets)
{
    std::string res;
    res += "\n.Lend" + func + ":\n";
    res += "add $" + std::to_string((offsets.size()) * 8) + ", %rsp\n";
    res += "pop %rbp\n";
    res += "ret\n\n\n";
    return res;
}

std::string Utils::GetNewLocalLabel()
{
    return ".L" + std::to_string(CurrentLabel++);
}