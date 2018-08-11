//
//  Function.hpp
    //  BabyWolf
//
//

#ifndef Function_hpp
#define Function_hpp

#include "AbstractFunction.hpp"
#include <iostream> //debug

class Function : public AbstractFunction {
    friend class AbstractFunction;
    friend class Variadic;
    friend class Unary;
    
    AbstractFunction* f;
    std::string name;
    
    virtual AbstractFunction* copy() const {
        return f->copy();
    }
    
    virtual const Function wrap() const {
        return f->wrap();
    }
    
    virtual const Function flatten() const {
        return f->flatten();
    }
    
    virtual const Function collapse() const {
        return f->collapse();
    }
    
    virtual std::string getPrefixString() const {
        return f->getPrefixString();
    }
    
    virtual std::string getInfixString() const {
        return f->getInfixString();
    }
    
public:
    //Create Argument(default), Constant, Unary, Variadic respectively
    Function();
    Function(double value);
    Function(OperationType op, Function fn);
    Function(OperationType op, std::vector<Function> fns);
    
    Function(std::nullptr_t nptr) : f(nptr) {}
    
    Function(const Function& orig) : f(orig.copy()), name(orig.name) {
    }
    
    virtual double evaluate(double arg) const {
        return f->evaluate(arg);
    }
    
    virtual Function substitute(const Function subFn) const {
        return f->substitute(subFn);
    }
    
    virtual Function derivative() const {
        return f->derivative();
    }
    
    virtual FunctionType getType() const {
        return f->getType();
    }
    
    virtual OperationType getOperation() const {
        return f->getOperation();
    }
    
    virtual std::pair<const Function, std::vector<Function>> getFns() const {
        return f->getFns();
    }
    
    bool isNull() const {
        return f == nullptr;
    }
    
    //returns function name
    std::string getName() const;

    //returns true if existing function with that name already exists
    bool setName(std::string name);

    //overload ostream operator to return string
    friend std::ostream& operator<<(std::ostream& o, const Function& fn);

    
    ~Function() {
        delete f;
    }
};

#endif /* Function_hpp */

