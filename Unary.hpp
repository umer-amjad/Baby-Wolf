//
//  Unary.hpp
//  BabyWolf
//
//

#ifndef Unary_hpp
#define Unary_hpp

#include "AbstractFunction.hpp"
#include "Constant.hpp"
#include "Function.hpp"

class Unary : public AbstractFunction {    
    OperationType op;
    const Function fn;

    AbstractFunction* copy() const override;
    const Function wrap() const override;
    const Function flatten() const override;
    const Function collapse() const override;
    std::string getPrefixString() const override;
    std::string getInfixString() const override;

public:
    Unary(std::string o, const Function fn);
    Unary(OperationType o, const Function fn);

    double evaluate(double arg) const override;
    Function substitute(const Function subFn) const override;
    Function derivative() const override;
    FunctionType getType() const override;
    OperationType getOperation() const override;
    std::pair<const Function, std::vector<Function>> getFns() const override;
};

#endif /* Unary_hpp */
