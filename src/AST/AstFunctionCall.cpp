//
// Created by saumonbro on 2/21/25.
//

#include <AST/AstFunctionCall.h>
#include <Utils.h>
#include <complex>
#include <utility>

long AstFunctionCall::Evaluate()
{
    return 0;
}

AstFunctionCall::AstFunctionCall(const std::shared_ptr<Function> &function,
                                 std::vector<std::unique_ptr<Ast>> &&args)
    : m_args(std::move(args))
{
    m_function = function;
}

std::string AstFunctionCall::Compile(ContextMap &offsets)
{
    std::string res;
    // compute args and move into registers
    for (size_t i = 0; i < m_args.size(); i++)
    {
        res += m_args[i]->Compile(offsets);
        res += Utils::MoveRegisterToRegister("%rax", args_reg[i]);
    }

    res += "call " + m_function->GetName() + "\n";
    return res;
}

std::string AstFunctionCall::Dump()
{
    return m_function->Dump();
}

std::unique_ptr<Ast> AstFunctionCall::Optimize()
{
    return Ast::Optimize();
}
