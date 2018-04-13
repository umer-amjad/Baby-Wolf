//
//  Argument.cpp
//  BabyWolf
//
//

#include "Argument.hpp"
#include "Constant.hpp"

Function* Argument::copy() const {
    return new Argument;
}

double Argument::evaluate(double arg) const {
    return arg;
}

Function* Argument::substitute(const Function* subFn) const {
    return subFn->copy();
}

Function* Argument::derivative() const {
    return new Constant(1);
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

