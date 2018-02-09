//
//  Constant.hpp
//  BabyWolf
//
//

#ifndef Constant_hpp
#define Constant_hpp

#include "Function.hpp"

class Constant: public Function{
    double val;
public:
    Constant(double val);
    virtual double eval(double arg) const;
    virtual Function* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Constant_hpp */
