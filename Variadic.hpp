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
#include "Flattener.hpp"

class Variadic: public Function{
    char op;
    std::vector<const Function*> fns;

    virtual const Function* wrap() const;
    virtual const Function* flatten() const;
    virtual const Function* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    
public:
    Variadic(char o, std::vector<const Function*> fns);
    virtual double eval(double arg) const;
    virtual FunctionType getType() const;
    virtual char getOp() const;
    virtual std::pair<const Function*, std::vector<const Function*>> getFns() const;
    ~Variadic();
};


#endif /* Variadic_hpp */
