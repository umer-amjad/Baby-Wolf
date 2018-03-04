//
//  Constant.cpp
//  BabyWolf
//
//

#include "Constant.hpp"

Constant::Constant(double val): val(val){}

Constant::Constant(const Constant& c){
    this->val = c.val;
}

Constant& Constant::operator=(Constant c){
    std::swap(*this, c);
    return *this;
}

Function* Constant::copy() const {
    return new Constant(*this);
}


double Constant::eval(double arg) const {
    return val;
}

Function* Constant::substitute(const Function* subFn) const {
    return new Constant(*this);
}

std::string Constant::getPrefixString() const {
    std::stringstream stream;
    if (val == M_PI){
        return "π";
    }
    if (val == M_E){
        return "e";
    }
    stream << val;
    return stream.str();
}

Function* Constant::collapse() const {
    return new Constant(val);
}

std::string Constant::getInfixString() const {
    return getPrefixString();
}

FunctionType Constant::getType() const {
    return FunctionType::CONSTANT;
}

