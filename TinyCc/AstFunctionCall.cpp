//
// Created by saumonbro on 2/21/25.
//

#include "AstFunctionCall.h"

long AstFunctionCall::Evaluate() {

    return 0;
}

AstFunctionCall::AstFunctionCall(const std::shared_ptr<Function>& function, const std::vector<Ast> &args) {
    m_function = function;
}

std::string AstFunctionCall::Compile(std::unordered_map<std::string, int> &offsets)
{
    std::string res;
    res += "call " + m_function->GetName() + "\n";
    return res;
}

std::string AstFunctionCall::Dump()
{
    return m_function->Dump();
}

std::unique_ptr<Ast> AstFunctionCall::Optimize() {
    return Ast::Optimize();
}
