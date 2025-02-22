//
// Created by saumonbro on 2/21/25.
//

#include "Function.h"

#include <utility>

#include "Utils.h"





Function::Function(std::string name, std::unique_ptr<AstBlock> body, const std::vector<std::string> &args,
    std::shared_ptr<std::map<std::string, std::unique_ptr<Ast>>> context)
: m_name(std::move(name)), m_body(std::move(body)), m_args(args), m_context(std::move(context))
{
}

// Args not in offset
std::string Function::Compile() const
{

    auto offsets = std::unordered_map<std::string, int>();
    int offset = 1;
    for (const auto& s : *m_context) {
        offsets[s.first] = offset * 8;
        offset++;
    }

    for (const auto& arg : m_args) {
        if (*m_context->find(arg) == *m_context->end()) {
            offsets[arg] = offset * 8;
            offset++;
        }
    }

    std::string res;
    res += m_name + ":\n\n";
    res += Utils::BuildPrologue(offsets);
    res += m_body->Compile(offsets);
    res += "\n";
    res += Utils::BuildEpilogue(offsets);
    return res;
}

std::string Function::Dump()
{
    std::string res;
    res += m_name + ":\n\n";
    res += m_body->Dump();
    res += "\n";
    return res;
}

void Function::Optimize()
{
    std::unique_ptr<Ast> cur = m_body->Optimize();
    if (cur == nullptr) {
        return;
    }
    m_body = std::move(cur);
}
