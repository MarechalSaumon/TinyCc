//
// Created by saumonbro on 3/8/25.
//

#ifndef METHOD_H
#define METHOD_H

#include <Function.h>



class Method final : public Function {
public:
    explicit Method(std::string name, const std::vector<std::string> &args,
                         std::shared_ptr<std::map<std::string, std::shared_ptr<Variable>>> context, std::shared_ptr<Context> global_context, bool isStatic, VariableType returnType = INTEGER) :
    Function(std::move(name), args, std::move(context),  std::move(global_context),isStatic, std::move(returnType))
    {

    }


};



#endif //METHOD_H
