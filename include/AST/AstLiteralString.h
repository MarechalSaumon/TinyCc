//
// Created by saumonbro on 3/6/25.
//

#ifndef ASTLITERALSTRING_H
#define ASTLITERALSTRING_H

#include "Ast.h"

class AstLiteralString : public Ast {
public:

    explicit AstLiteralString(const std::string& value)
          : value(value)
    {}

    ValueType GetValueType() override;

    long Evaluate() override;
    std::string Compile(ContextMap &offsets) override;
    std::string Dump() override;
    VariableType UnderlyingType() override;

private:
    std::string value;
};



#endif //ASTLITERALSTRING_H
