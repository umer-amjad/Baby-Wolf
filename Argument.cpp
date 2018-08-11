//
//  Argument.cpp
//  BabyWolf
//
//

#include "Argument.hpp"
#include "Constant.hpp"
#include "Function.hpp"

AbstractFunction* Argument::copy() const {
    return new Argument(); //new Arg
}

double Argument::evaluate(double arg) const {
    return arg;
}

Function Argument::substitute(const Function subFn) const {
    return subFn; //return a copy of subFn
}

Function Argument::derivative() const {
    return Function(1);
}

const Function Argument::wrap() const {
    return Function(); //new Argument
}

const Function Argument::flatten() const {
    return Function(); //new Argument
}

const Function Argument::collapse() const {
    return Function(); //new Argument
}

std::string Argument::getPrefixString() const {
    return "x";
}

std::string Argument::getInfixString() const {
    return getPrefixString();
}

FunctionType Argument::getType() const {
    return FunctionType::ARGUMENT;
}

