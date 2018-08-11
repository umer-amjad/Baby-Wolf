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

    virtual AbstractFunction* collapse() const;
    virtual AbstractFunction* copy() const;

public:
    Constant(double val);
    Constant(const Constant& c);

    Constant& operator=(Constant c);

    virtual double evaluate(double arg) const;
    virtual AbstractFunction* substitute(const AbstractFunction* subFn) const;
    virtual AbstractFunction* derivative() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Constant_hpp */
