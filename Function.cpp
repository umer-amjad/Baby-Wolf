//
//  Function.cpp
//  BabyWolf
//
//

#include <algorithm>

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

//compare simplified functions
bool operator<(const Function& f1, const Function& f2){
    if (f1.getType() != f2.getType()) {
        return f1.getType() < f2.getType();
    } else if (f1.getType() == FunctionType::CONSTANT) {
        return f1.getValue() < f2.getValue();
    } else if (f1.getType() == FunctionType::ARGUMENT) {
        return false; //arguments are always equal
    } else if (f1.getType() == FunctionType::UNARY) {
        if (f1.getOperation() == f2.getOperation()) {
            return f1.getFns().first < f2.getFns().first;
        } else {
            return f1.getOperation() < f2.getOperation();
        }
    } else if (f1.getType() == FunctionType::VARIADIC) {
        if (f1.getOperation() == f2.getOperation()) {
            //lexicographical comparison
            const std::vector<Function>& fns1 = f1.getFns().second;
            const std::vector<Function>& fns2 = f2.getFns().second;
            return std::lexicographical_compare(fns1.begin(), fns1.end(), fns2.begin(), fns2.end());
        } else {
            return f1.getOperation() < f2.getOperation();
        }        
    }
}