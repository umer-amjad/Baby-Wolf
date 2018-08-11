//
//  Function.cpp
//  BabyWolf
//
//

#include "Function.hpp"
#include "Argument.hpp"
#include "Constant.hpp"
#include "Unary.hpp"
#include "Variadic.hpp"

Function::Function() : f(new Argument()) {}
Function::Function(double value) : f(new Constant(value)) {}
Function::Function(OperationType op, Function fn) : f(new Unary(op, fn)) {}
Function::Function(OperationType op, std::vector<Function> fns) : f(new Variadic(op, fns)) {}

const Function Function::simplify() const {
    const Function simplified = this->wrap().flatten().collapse().flatten();
    //userFunctions.push_back(simplified);
    return simplified;
}

std::string Function::getName() const {
    return name;
}

bool Function::setName(std::string name){
    this->name = name;
    auto userFnIter = userFunctions.find(name);
    if (userFnIter != userFunctions.end()){
        userFunctions.erase(userFnIter);
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& o, const Function& fn){
    std::string name = fn.name;
    if (name == ""){
        name = "unnamed";
    }
    if (AbstractFunction::opts.prefix){
        o << "(define (" << name << " x) " << fn.getPrefixString() << ")\n";
    }
    if (AbstractFunction::opts.infix){
        o << name << "(x) = " << fn.getInfixString() << '\n';
    }
    return o;
}