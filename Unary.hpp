//
//  Unary.hpp
//  BabyWolf
//
//

#ifndef Unary_hpp
#define Unary_hpp

#include "AbstractFunction.hpp"
#include "Constant.hpp"

class Unary : public AbstractFunction {
    OperationType op;
    const AbstractFunction* fn;

    virtual AbstractFunction* copy() const;
    virtual const AbstractFunction* wrap() const;
    virtual const AbstractFunction* flatten() const;
    virtual const AbstractFunction* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;

public:
    Unary(std::string o, const AbstractFunction* fn);
    Unary(OperationType o, const AbstractFunction* fn);
    Unary(const Unary& u);

    Unary& operator=(Unary u);

    virtual double evaluate(double arg) const;
    virtual AbstractFunction* substitute(const AbstractFunction* subFn) const;
    virtual AbstractFunction* derivative() const;
    virtual FunctionType getType() const;
    virtual OperationType getOperation() const;
    virtual std::pair<const AbstractFunction*, std::vector<const AbstractFunction*>> getFns() const;
    ~Unary();
};

#endif /* Unary_hpp */
