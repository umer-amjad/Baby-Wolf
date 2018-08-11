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

class Variadic : public AbstractFunction {
    OperationType op;
    std::vector<const AbstractFunction*> fns;

    virtual AbstractFunction* copy() const;
    virtual const AbstractFunction* wrap() const;
    virtual const AbstractFunction* flatten() const;
    virtual const AbstractFunction* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;

public:
    Variadic(std::string o, std::vector<const AbstractFunction*> fns);
    Variadic(OperationType o, std::vector<const AbstractFunction*> fns);
    Variadic(const Variadic& v);

    Variadic& operator=(Variadic v);

    virtual double evaluate(double arg) const;
    virtual AbstractFunction* substitute(const AbstractFunction* subFn) const;
    virtual AbstractFunction* derivative() const;
    virtual FunctionType getType() const;
    virtual OperationType getOperation() const;
    virtual std::pair<const AbstractFunction*, std::vector<const AbstractFunction*>> getFns() const;
    ~Variadic();
};


#endif /* Variadic_hpp */
