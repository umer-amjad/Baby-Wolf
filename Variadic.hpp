//
//  Variadic.hpp
//  BabyWolf
//
//

#ifndef Variadic_hpp
#define Variadic_hpp

#include <vector>

#include "Function.hpp"
#include "Unary.hpp"
#include "Constant.hpp"

class Variadic: public Function{
    char op;
    std::vector<const Function*> fns;

    virtual Function* copy() const;
    virtual const Function* wrap() const;
    virtual const Function* flatten() const;
    virtual const Function* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    
public:
    Variadic(char o, std::vector<const Function*> fns);
    Variadic(const Variadic& v);
    
    Variadic& operator=(Variadic v);
    
    virtual double eval(double arg) const;
    virtual Function* substitute(const Function* subFn) const;
    virtual FunctionType getType() const;
    virtual char getOp() const;
    virtual std::pair<const Function*, std::vector<const Function*>> getFns() const;
    ~Variadic();
};


#endif /* Variadic_hpp */
