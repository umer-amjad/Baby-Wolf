//
//  Unary.hpp
//  BabyWolf
//
//

#ifndef Unary_hpp
#define Unary_hpp

#include "Function.hpp"
#include "Constant.hpp"

class Unary: public Function{
    std::string op;
    const Function* fn;

    virtual Function* copy() const;
    virtual const Function* wrap() const;
    virtual const Function* flatten() const;
    virtual const Function* collapse() const;
    virtual std::string getPrefixString() const;
    virtual std::string getInfixString() const;
    
public:
    Unary(std::string o, const Function* fn);
    Unary(const Unary& u);
    
    Unary& operator=(Unary u);

    virtual double eval(double arg) const;
    virtual const Function* substitute(const Function* subFn) const;
    virtual FunctionType getType() const;
    virtual char getOp() const;
    virtual std::pair<const Function*, std::vector<const Function*>> getFns() const;
    ~Unary();
};

#endif /* Unary_hpp */
