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

    virtual AbstractFunction* copy() const;
    virtual const Function wrap() const;
    virtual const Function flatten() const;
    virtual const Function collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;

public:
    Variadic(std::string o, std::vector<Function> fns);
    Variadic(OperationType o, std::vector<Function> fns);

    virtual double evaluate(double arg) const;
    virtual Function substitute(const Function subFn) const;
    virtual Function derivative() const;
    virtual FunctionType getType() const;
    virtual OperationType getOperation() const;
    virtual std::pair<const Function, std::vector<Function>> getFns() const;
};


#endif /* Variadic_hpp */
