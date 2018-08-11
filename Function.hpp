//
//  Function.hpp
    //  BabyWolf
//
//

#ifndef Function_hpp
#define Function_hpp

#include "AbstractFunction.hpp"

class Function : public AbstractFunction {
    AbstractFunction* f;
    
    virtual AbstractFunction* copy() const;
    
    virtual const AbstractFunction* wrap() const {
        return f->wrap();
    }
    
    virtual const AbstractFunction* flatten() const {
        return f->flatten();
    }
    
    virtual const AbstractFunction* collapse() const {
        return f->collapse();
    }
    
    virtual std::string getPrefixString() const {
        return f->getPrefixString();
    }
    
    virtual std::string getInfixString() const {
        return f->getInfixString();
    }
    
public:
    Function(AbstractFunction* f) : f(f) {}
    
    Function(const Function& orig) {
        f = orig.copy();
    }

    virtual double evaluate(double arg) const {
        return f->evaluate(arg);
    }
    
    virtual AbstractFunction* substitute(const AbstractFunction* subFn) const {
        return f->substitute(subFn);
    }
    
    virtual AbstractFunction* derivative() const {
        return f->derivative();
    }
    
    virtual FunctionType getType() const {
        return f->getType();
    }
    
    virtual OperationType getOperation() const {
        return f->getOperation();
    }
    
    virtual std::pair<const AbstractFunction*, std::vector<const AbstractFunction*>> getFns() const {
        return f->getFns();
    }

    ~Function() {
        delete f;
    }
};

#endif /* Function_hpp */

