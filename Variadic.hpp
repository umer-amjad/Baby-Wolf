//
//  Variadic.hpp
//  BabyWolf
//
//

#ifndef Variadic_hpp
#define Variadic_hpp

#include <vector>

#include "AbstractFunction.hpp"
#include "Unary.hpp"
#include "Constant.hpp"
#include "Function.hpp"

class Variadic : public AbstractFunction {    
    OperationType op;
    std::vector<Function> fns;

    AbstractFunction* copy() const override;
    const Function wrap() const override;
    const Function flatten() const override;
    const Function collapse() const override;
    std::string getPrefixString() const override;
    std::string getInfixString() const override;

public:
    Variadic(std::string o, std::vector<Function> fns);
    Variadic(OperationType o, std::vector<Function> fns);

    double evaluate(double arg) const override;
    Function substitute(const Function subFn) const override;
    Function derivative() const override;
    FunctionType getType() const override;
    OperationType getOperation() const override;
    std::pair<const Function, std::vector<Function>> getFns() const override;
};


#endif /* Variadic_hpp */
