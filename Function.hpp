//
//  Function.hpp
//  BabyWolf
//
//

#ifndef Function_hpp
#define Function_hpp

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip> //for decimal output
#include <cmath>
#include <memory>
#include <vector>
#include <utility>

enum FunctionType {VARIADIC, UNARY, ARGUMENT, CONSTANT};

class Function{
    friend class Variadic;
    friend class Unary;
    
    //wraps use of "-" and "/" variadics with "neg" and "inv" unaries - not new-ly allocated except for new unaries
    virtual const Function* wrap() const {
        return this;
    };
    
    //flattens syntax tree for operations + and * - not new-ly allocated
    virtual const Function* flatten() const {
        return this;
    };
    
    //"collapses" tree where arithmetic operations where possible - NOTE: returns new-ly allocated function
    virtual const Function* collapse() const = 0;
    
    //return function string in getPrefixString notation
    virtual std::string getPrefixString() const = 0 ;
    
    //return function string in infix notation
    virtual std::string getInfixString() const = 0;
    
    //return function type
    virtual FunctionType getType() const = 0;
    
    //get operation (for variadic and unary types only), default empty:
    virtual char getOp() const {return ' ';};
    
    //get functions (for variadic and unary types only), default <nullptr, empty vector>:
    virtual std::pair<const Function*, std::vector<const Function*>> getFns() const {
        std::vector<const Function*> empty;
        return {nullptr, empty};
    };
    
public:
    //evaluate function given argument
    virtual double eval(double arg) const = 0;
    
    //returns arithmetically simplified function - NOTE: returns new-ly allocated function
    const Function* simplify() const {
        return this->wrap()->flatten()->collapse()->flatten();
    };
    
    //overload ostream operator to return string (switch to getPrefixString here if wanted)
    friend std::ostream& operator<< (std::ostream& o, const Function& fn){
        o << fn.getPrefixString();
        return o;
    };
    
    //virtual destructor
    virtual ~Function(){};
};

#endif /* Function_hpp */
