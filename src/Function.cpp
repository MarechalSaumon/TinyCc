//
// Created by saumonbro on 2/21/25.
//

#include <Context.h>
#include <Function.h>
#include <iostream>
#include <Utils.h>
#include <list>
#include <utility>
#include <AST/AstReference.h>
#include <Variables/Variable.h>

Function::Function(std::string name, const std::vector<std::string> &args,
                   std::shared_ptr<std::map<std::string, std::shared_ptr<Variable> > > context,
                   std::shared_ptr<Context> global_context, bool isStatic, VariableType returnType)
    : m_name(std::move(name))
      , m_args(args)
      , m_isStatic(isStatic)
      , m_context(std::move(context))
      , m_returnType(std::move(returnType))
      , global_context(std::move(global_context))
{
}

// Args not in offset
std::string Function::Compile()
{
    static const std::vector<std::string> args_reg = {
        "%rdi", "%rsi", "%rdx",
        "%rcx", "%r8", "%r9"
    };
    std::unordered_map<std::string, int> offsets;
    int offset = 1;
    for (const auto &[identifier, variable]: *m_context)
    {
        offsets[identifier] = offset * 8;
        variable->SetOffset(offset * 8);
        offset++;
    }

    // compute offsets for each argument
    for (const auto &arg: m_args)
    {
        if (!m_context->contains(arg))
        {
            offsets[arg] = offset * 8;
            offset++;
        }
    }

    std::string res;
    res += m_name + ":\n\n";
    res += Utils::BuildPrologue(offsets);

    // move args on the stack

    for (size_t i = 0; i < m_args.size(); i++)
    {
        res += Utils::MoveRegisterToStack(args_reg[i], offsets[m_args[i]]);
    }

    Context context{};
    context.SetOffsets(m_context);
    res += m_body->Compile(context);

    res += "\n";

    res += Utils::BuildEpilogue(m_name, offsets);
    return res;
}

std::string Function::Dump() const
{
    std::string res;
    res += m_name + ":\n\n";
    res += m_body->Dump();
    res += "\n";
    return res;
}

std::string Function::GetPrototype() const
{
    std::string res = (m_isStatic ? "static " : "") + m_name + "(";

    for (size_t i = 0; i < m_args.size(); i++)
    {
        const auto &s = m_args[i];
        res += (VariableTypeToString((*m_context)[s]->GetType()) + " ");
        res += s;
        if (i != m_args.size() - 1)
            res += " ";
    }

    return res + ");";
}

void Function::Optimize()
{
    Logger::Log("Optimizing function", DEBUG);
    std::unique_ptr<Ast> cur = m_body->Optimize();
    if (cur == nullptr)
    {
        return;
    }
    m_body = std::move(cur);
}

bool Function::Returns() const
{
    if (m_returnType == VOID)
    {
        return !m_body->Returns();
    }
    return m_body->Returns();
}

void Function::AddVariable(const std::string &name, std::shared_ptr<Variable> variable) const
{
    (*m_context)[name] = std::move(variable);
}
