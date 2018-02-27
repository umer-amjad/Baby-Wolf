//
//  Argument.cpp
//  BabyWolf
//
//

#include "Argument.hpp"

double Argument::eval(double arg) const {
    return arg;
}

const Function* Argument::substitute(const Function* subFn) const {
    return subFn;
}

Function* Argument::collapse() const {
    return new Argument;
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

