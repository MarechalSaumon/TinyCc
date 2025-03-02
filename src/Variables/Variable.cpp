//
// Created by saumonbro on 2/23/25.
//

#include <Variables/Variable.h>
#include <vector>
#include <Variables/IntegerVariable.h>
#include <Variables/StringVariable.h>

Variable::Variable(const int offset, std::string m_name, std::unique_ptr<Ast> ref) :
    m_offset(offset)
    , m_name(std::move(m_name))
    , m_referenced(std::move(ref))
{
}

void Variable::SetAst(std::unique_ptr<Ast> ref)
{
    m_referenced = std::move(ref);
}

std::string Variable::GetAssemblyAlias() const
{
    if (m_offset > 0)
    {
        return std::to_string(-m_offset) + "(" + Utils::StackPtr() + ")";
    }
    static const std::vector<std::string> args_reg = {"%rdi", "%rsi", "%rdx",
                                                      "%rcx", "%r8", "%r9"};
    return args_reg[-m_offset]; // weirdo
}

void Variable::SetOffset(const int offset)
{

    m_offset = offset;

}

std::shared_ptr<Variable> make_variable(int offset, std::string m_name, VariableType type)
{
    if (type == STRING)
        return std::make_shared<StringVariable>(offset, m_name);
    return std::make_shared<IntegerVariable>(offset, m_name);
}