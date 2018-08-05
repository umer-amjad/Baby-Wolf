//
//  Constant.hpp
//  BabyWolf
//
//

#ifndef Constant_hpp
#define Constant_hpp

#include "Function.hpp"

class Constant : public Function {
    double val;

    virtual Function* collapse() const;
    virtual Function* copy() const;

public:
    Constant(double val);
    Constant(const Constant& c);

    Constant& operator=(Constant c);

    virtual double evaluate(double arg) const;
    virtual Function* substitute(const Function* subFn) const;
    virtual Function* derivative() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Constant_hpp */
