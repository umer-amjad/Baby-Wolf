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

    const Function wrap() const override;
    const Function flatten() const override;
    const Function collapse() const override;
    AbstractFunction* copy() const override;

public:
    Constant(double val);
    Constant(const Constant& c);

    Constant& operator=(Constant c);

    double evaluate(double arg) const override;
    Function substitute(const Function subFn) const override;
    Function derivative() const override;
    std::string getPrefixString() const override;
    std::string getInfixString() const override;
    FunctionType getType() const override;
    double getValue() const override;
};

#endif /* Constant_hpp */
