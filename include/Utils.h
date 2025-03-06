//
// Created by saumonbro on 2/21/25.
//

#ifndef UTILS_H
#define UTILS_H
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>



class Utils
{
public:
    static std::string CurrentStringLabel;

    static std::string GetNexStringLabel();

    static std::string LogicalNot(const std::string &reg);

    static std::string LogicalOr(const std::string &reg1,
                                 const std::string &reg2);
    static std::string StackPtr();
    static std::string MoveRegisterToStack(const std::string &reg, int offset);
    static std::string MoveRegisterToRegister(const std::string &reg1,
                                              const std::string &reg2);
    static std::string MoveToRax(int offset);

    static std::string MoveToRax(const std::string &lhs);

    static std::string MoveFromRax(int offset);

    static std::string MoveFromRax(const std::string &rhs);

    static std::string MoveLiteralToStack(const std::string &literal,
                                          int offset);

    static std::string MoveLiteralToStack(const std::string &literal, const std::string &name);

    static std::string MoveLiteralToRax(long value);

    static std::string MoveLiteralToRax(const std::string &value);

    static std::string
    BuildPrologue(const std::unordered_map<std::string, int> &offsets);

    static std::string
    BuildEpilogue(const std::string &func,
                  const std::unordered_map<std::string, int> &offsets);

    static std::string GetNewLocalLabel();

private:
    static unsigned CurrentLabel;
};

#endif // UTILS_H
