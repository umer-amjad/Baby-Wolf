//
//  Argument.hpp
//  BabyWolf
//
//

#ifndef Argument_hpp
#define Argument_hpp

#include "Function.hpp"

class Argument: public Function{
    virtual Function* collapse() const;
    virtual Function* copy() const;
public:
    virtual double eval(double arg) const;
    virtual const Function* substitute(const Function* subFn) const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Argument_hpp */
