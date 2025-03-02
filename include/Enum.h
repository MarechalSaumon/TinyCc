//
// Created by saumonbro on 3/2/25.
//

#ifndef ENUM_H
#define ENUM_H
#include <stdexcept>
#include <string>


enum VariableType
{
    INTEGER,
    STRING,
    NONE,
};

inline std::string VariableTypeToString(VariableType type)
{
    switch (type)
    {
        case INTEGER:
            return "INTEGER";
        case STRING:
            return "STRING";
        case NONE:
            return "NONE";
        default:
            throw std::runtime_error("Invalid variable type: " + std::to_string(type));
    }
}


#endif //ENUM_H
