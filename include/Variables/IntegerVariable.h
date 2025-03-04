//
// Created by saumonbro on 3/2/25.
//

#ifndef INTEGERVARIABLE_H
#define INTEGERVARIABLE_H

#include "Variable.h"

class IntegerVariable : public Variable {

    public:
        IntegerVariable(int offset, std::string m_name, std::unique_ptr<Ast> ref = std::make_unique<AstLiteral>(0));

        VariableType GetType() const override;

};



#endif //INTEGERVARIABLE_H
