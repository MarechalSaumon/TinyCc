//
// Created by saumonbro on 3/4/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H
#include <map>

#include <Class.h>
#include <set>
#include <utility>

class Context {
    public:

        Context() : ro_data(), classes_()
        {

        }

        Class& GetClasse(const std::string& class_name)
        {
            for (const auto& classes : classes_)
            {
                if (classes->GetName() == class_name)
                {
                    return *classes;
                }
            }
            throw std::invalid_argument("Class '" + class_name + "' not found");
        }
        std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> GetOffsets() const
            {
                return offsets;
            }

    void SetOffsets(std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> offsets)
        {
            this->offsets = std::move(offsets);
        }

    void AddClass(std::unique_ptr<Class> cls)
        {
            classes_.insert(std::move(cls));
        }



    private:
        // Global context
        std::set<std::string> ro_data;
        std::set<std::unique_ptr<Class>> classes_;
        // Function dependant
        std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> offsets;
};

#endif //CONTEXT_H
