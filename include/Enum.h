//
// Created by saumonbro on 3/2/25.
//

#ifndef ENUM_H
#define ENUM_H
#include <stdexcept>
#include <string>
#include <variant>

enum EnumVariableType
{
    INTEGER,
    STRING,
    VOID,
    NONE,
};

struct VariableType
{

    VariableType(const std::string& variable) : value(variable) {}

    VariableType(EnumVariableType variable) : value(variable) {}


    bool operator==(const EnumVariableType &val) const
    {
        if (std::holds_alternative<EnumVariableType>(value))
        {
            return std::get<EnumVariableType>(value) == val;
        }
        return false;
    }

    bool operator==(const std::string &val) const
    {
        if (std::holds_alternative<std::string>(value))
        {
            return std::get<std::string>(value) == val;
        }
        return false;
    }

    std::variant<EnumVariableType, std::string> value;

};




inline std::string VariableTypeToString(const VariableType& type)
{
    if (std::holds_alternative<EnumVariableType>(type.value))
    {
        EnumVariableType value = std::get<EnumVariableType>(type.value);
        switch (value)
        {
            case INTEGER:
                return "int";
            case STRING:
                return "string";
            case NONE:
                return "NONE";
            default:
                throw std::runtime_error("Invalid variable type: ");
        }
    }
    return std::get<std::string>(type.value);

}


#endif //ENUM_H
