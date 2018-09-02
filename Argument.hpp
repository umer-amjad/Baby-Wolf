//
//  Argument.hpp
//  BabyWolf
//
//

#ifndef Argument_hpp
#define Argument_hpp

#include "AbstractFunction.hpp"

class Argument : public AbstractFunction {
    const Function wrap() const override;
    const Function flatten() const override;
    const Function collapse() const override;
    AbstractFunction* copy() const override;
public:
    double evaluate(double arg) const override;
    Function substitute(const Function subFn) const override;
    Function derivative() const override;
    std::string getPrefixString() const override;
    std::string getInfixString() const override;
    FunctionType getType() const override;
};

#endif /* Argument_hpp */
