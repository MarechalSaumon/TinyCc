//
// Created by saumonbro on 3/2/25.
//

#include <Variables/IntegerVariable.h>

IntegerVariable::IntegerVariable(int offset, std::string m_name, std::unique_ptr<Ast> ref) : Variable(offset, std::move(m_name), std::move(ref))
{
}
