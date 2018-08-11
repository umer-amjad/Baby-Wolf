//
//  Argument.hpp
//  BabyWolf
//
//

#ifndef Argument_hpp
#define Argument_hpp

#include "AbstractFunction.hpp"

class Argument : public AbstractFunction {
    virtual AbstractFunction* collapse() const;
    virtual AbstractFunction* copy() const;
public:
    virtual double evaluate(double arg) const;
    virtual AbstractFunction* substitute(const AbstractFunction* subFn) const;
    virtual AbstractFunction* derivative() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Argument_hpp */
