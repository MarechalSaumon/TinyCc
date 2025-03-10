//
// Created by saumonbro on 3/8/25.
//

#ifndef CLASS_H
#define CLASS_H
#include <Enum.h>
#include <memory>
#include <Method.h>
#include <set>
#include <utility>
#include <vector>


struct Attribute
{

    VariableType type;
    std::string name;
    bool is_public;

public:

    Attribute(VariableType type, const std::string &name, bool is_public) : type(std::move(type)), name(name), is_public(is_public)
    {
    }
};

class Class
{
public:
    Class(std::set<std::shared_ptr<Method> > methods, std::vector<Attribute> attributes,
          const std::string &name) : methods_(std::move(methods)), attributes_(std::move(attributes)), name_(name)
    {
    }

    std::string GetName() const
    {
        return name_;
    }



    std::variant<Attribute, std::shared_ptr<Method>> GetAttribute(const std::string &name) const
    {
        for (const auto& attr : attributes_)
        {
            if (attr.name == name)
                return attr;
        }
        for (const auto &attr : methods_)
        {
            if (attr->GetName() == name)
            {
                return attr;
            }
        }
        throw std::runtime_error("Class::GetAttribute(const std::string&) attribute not found");
    }

    std::string GetAssemblyName(const std::string& attribute, const std::string& base) const
    {
        unsigned long offset = 0;
        for (offset = 0; offset < attributes_.size(); ++offset)
        {
            if (attributes_[offset].name == attribute)
            {
                break;
            }
        }
        return std::to_string(offset * 8) + "(" + base + ")";
    }

private:
    std::set<std::shared_ptr<Method>> methods_;
    std::vector<Attribute> attributes_;
    std::string name_;
};

#endif //CLASS_H
