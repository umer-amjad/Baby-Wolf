//
//  Argument.cpp
//  BabyWolf
//
//

#include "Argument.hpp"
#include "Constant.hpp"

AbstractFunction* Argument::copy() const {
    return new Argument;
}

double Argument::evaluate(double arg) const {
    return arg;
}

AbstractFunction* Argument::substitute(const AbstractFunction* subFn) const {
    return subFn->copy();
}

AbstractFunction* Argument::derivative() const {
    return new Constant(1);
}

AbstractFunction* Argument::collapse() const {
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

