//
// Created by saumonbro on 3/2/25.
//

#include <Variables/StringVariable.h>

#include <utility>


StringVariable::StringVariable(int offset, std::string m_name, std::unique_ptr<Ast> ref) : Variable(offset, std::move(m_name), std::move(ref))
{
}

std::shared_ptr<Variable> StringVariable::operator+(const Variable &other) const
{
    if (other.GetType() == STRING) // concat strings
    {
        
    }
    else // itoa + concat strings
    {

    }
    return nullptr;
}

std::shared_ptr<Variable> StringVariable::operator*(const Variable &other) const
{
    return Variable::operator*(other);
}

VariableType StringVariable::GetType() const
{
    return STRING;
}
