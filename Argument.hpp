//
//  Argument.hpp
//  BabyWolf
//
//

#ifndef Argument_hpp
#define Argument_hpp

#include "AbstractFunction.hpp"

class Argument : public AbstractFunction {
    virtual const Function wrap() const;
    virtual const Function flatten() const;
    virtual const Function collapse() const;
    virtual AbstractFunction* copy() const;
public:
    virtual double evaluate(double arg) const;
    virtual Function substitute(const Function subFn) const;
    virtual Function derivative() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Argument_hpp */
