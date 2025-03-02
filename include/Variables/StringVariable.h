//
// Created by saumonbro on 3/2/25.
//

#ifndef STRINGVARIABLE_H
#define STRINGVARIABLE_H

#include "Variable.h"

class StringVariable : public Variable {
    public:
    StringVariable(int offset, std::string m_name, std::unique_ptr<Ast> ref = std::make_unique<AstLiteral>(0));

    std::shared_ptr<Variable> operator+(const Variable &other) const override;
    std::shared_ptr<Variable> operator*(const Variable &other) const override;
    VariableType GetType() const override;
};



#endif //STRINGVARIABLE_H
