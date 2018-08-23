//
//  Constant.cpp
//  BabyWolf
//
//

#include "Constant.hpp"
#include "Function.hpp"

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
    return new Constant(val);
}

double Constant::evaluate(double arg) const {
    return val;
}

Function Constant::substitute(const Function subFn) const {
    return Function(val);
}

Function Constant::derivative() const {
    return Function(0.0);
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

const Function Constant::wrap() const {
    return Function(val);
}

const Function Constant::flatten() const {
    return Function(val);
}

const Function Constant::collapse() const {
    return Function(val);
}

std::string Constant::getInfixString() const {
    return getPrefixString();
}

FunctionType Constant::getType() const {
    return FunctionType::CONSTANT;
}

double Constant::getValue() const {
    return val;
}

