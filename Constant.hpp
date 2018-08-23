//
//  Constant.hpp
//  BabyWolf
//
//

#ifndef Constant_hpp
#define Constant_hpp

#include "AbstractFunction.hpp"

class Constant : public AbstractFunction {
    double val;

    virtual const Function wrap() const;
    virtual const Function flatten() const;
    virtual const Function collapse() const;
    virtual AbstractFunction* copy() const;

public:
    Constant(double val);
    Constant(const Constant& c);

    Constant& operator=(Constant c);

    virtual double evaluate(double arg) const;
    virtual Function substitute(const Function subFn) const;
    virtual Function derivative() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
    virtual double getValue() const;
};

#endif /* Constant_hpp */
