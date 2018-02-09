//
//  Argument.hpp
//  BabyWolf
//
//

#ifndef Argument_hpp
#define Argument_hpp

#include "Function.hpp"

class Argument: public Function{
public:
    virtual double eval(double arg) const;
    virtual Function* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    virtual FunctionType getType() const;
};

#endif /* Argument_hpp */
