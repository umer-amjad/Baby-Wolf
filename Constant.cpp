//
//  Constant.cpp
//  BabyWolf
//
//

#include "Constant.hpp"

#include <sstream>

Constant::Constant(double val) : val(val) {
}

Constant::Constant(const Constant& c) {
    this->val = c.val;
}

Constant& Constant::operator=(Constant c) {
    std::swap(*this, c);
    return *this;
}

AbstractFunction* Constant::copy() const {
    return new Constant(*this);
}

double Constant::evaluate(double arg) const {
    return val;
}

AbstractFunction* Constant::substitute(const AbstractFunction* subFn) const {
    return new Constant(*this);
}

AbstractFunction* Constant::derivative() const {
    return new Constant(0);
}

std::string Constant::getPrefixString() const {
    std::stringstream stream;
    if (val == M_PI) {
        return "π";
    }
    if (val == M_E) {
        return "e";
    }
    stream << val;
    return stream.str();
}

AbstractFunction* Constant::collapse() const {
    return new Constant(val);
}

std::string Constant::getInfixString() const {
    return getPrefixString();
}

FunctionType Constant::getType() const {
    return FunctionType::CONSTANT;
}

