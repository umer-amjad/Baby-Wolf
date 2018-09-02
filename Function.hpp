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
    
    AbstractFunction* copy() const override {
        return f->copy();
    }
    
    const Function wrap() const override {
        return f->wrap();
    }
    
    const Function flatten() const override {
        return f->flatten();
    }
    
    const Function collapse() const override {
        return f->collapse();
    }
    
    std::string getPrefixString() const override {
        return f->getPrefixString();
    }
    
    std::string getInfixString() const override {
        return f->getInfixString();
    }
    
    FunctionType getType() const override {
        return f->getType();
    }
    
    double getValue() const override {
        return f->getValue();
    }
    
    OperationType getOperation() const override {
        return f->getOperation();
    }
    
    std::pair<const Function, std::vector<Function>> getFns() const override {
        return f->getFns();
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
    
    Function& operator=(const Function& orig) {
        delete f;
        f = orig.copy();
        name = orig.name;
        return *this;
    }
    
    Function(Function&& moved) : f(moved.f) {
        name = std::move(moved.name);
        moved.f = nullptr;
    } 
    
    Function& operator=(Function&& moved) {
        delete f;
        f = moved.f;
        name = std::move(moved.name);
        moved.f = nullptr;
        return *this;
    }
    
    
    double evaluate(double arg) const override {
        return f->evaluate(arg);
    }
    
    Function substitute(const Function subFn) const override {
        return f->substitute(subFn);
    }
    
    Function derivative() const override {
        return f->derivative();
    }
    
    //returns arithmetically simplified function - NOTE: returns new-ly allocated function
    const Function simplify() const;
    
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

