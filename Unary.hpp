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

    virtual AbstractFunction* copy() const;
    virtual const Function wrap() const;
    virtual const Function flatten() const;
    virtual const Function collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;

public:
    Unary(std::string o, const Function fn);
    Unary(OperationType o, const Function fn);

    virtual double evaluate(double arg) const;
    virtual Function substitute(const Function subFn) const;
    virtual Function derivative() const;
    virtual FunctionType getType() const;
    virtual OperationType getOperation() const;
    virtual std::pair<const Function, std::vector<Function>> getFns() const;
};

#endif /* Unary_hpp */
