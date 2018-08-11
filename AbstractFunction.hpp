//
//  AbstractFunction.hpp
//  BabyWolf
//
//

#ifndef AbstractFunction_hpp
#define AbstractFunction_hpp

#include <cmath>
#include <string>
#include <iosfwd> //forward declare iostream and stringstream
#include <map>
#include <vector>
#include <utility> // for pairs

constexpr double M_PI = 3.14159265358979323846;  // pi 
constexpr double M_E = 2.71828182845904523536;  // e

enum FunctionType {
    VARIADIC = 0, UNARY, ARGUMENT, CONSTANT
};

class Function;

enum OperationType {
    PLUS = 0, MINUS, TIMES, DIVIDE, POWER, //5
    NEG, INV, ABS, //3
    LN, LOG, //2
    SIN, COS, TAN, SEC, CSC, COT, //6
    ASIN, ACOS, ATAN, ASEC, ACSC, ACOT, //6
    SINH, COSH, TANH, SECH, CSCH, COTH, //6
    ASINH, ACOSH, ATANH, ASECH, ACSCH, ACOTH, //6,
    INVALID //1 - 35 in total
};

struct Options {
    bool prefix;
    bool infix;
    bool simplify;
    bool evaluate;
    double base = M_E;
};

class AbstractFunction {
    friend class Variadic;
    friend class Unary;
    friend class Argument;
    friend class Function;

    //wraps use of "-" and "/" variadics with "neg" and "inv" unaries - not new-ly allocated except for new unaries
    virtual const Function wrap() const = 0;

    //flattens syntax tree for operations + and * - not new-ly allocated
    virtual const Function flatten() const = 0;

    //"collapses" tree for arithmetic operations where possible - NOTE: returns new-ly allocated function
    virtual const Function collapse() const = 0;

    //return function string in getPrefixString notation
    virtual std::string getPrefixString() const = 0;

    //return function string in infix notation
    virtual std::string getInfixString() const = 0;

    //return function type
    virtual FunctionType getType() const = 0;

    //get operation (for variadic and unary types only), default empty:
    virtual OperationType getOperation() const;

    //get functions (for variadic and unary types only), default <nullptr, empty vector>:
    virtual std::pair<const Function, std::vector<Function>> getFns() const;

    virtual AbstractFunction* copy() const = 0;

public:
    //Display options
    static Options opts;

    //functions defined by the user already
    static std::map<std::string, const Function> userFunctions;

    //maps to and from internal operation to string
    static std::map<OperationType, std::string> operationToString;
    static std::map<std::string, OperationType> stringToOperationType;

    //initialization of maps
    static void initalizeOperationTypeMaps();

    //evaluate function given argument
    virtual double evaluate(double arg) const = 0;

    //return this function but subtitute x with fn
    virtual Function substitute(const Function subFn) const = 0;

    //returns derivative of function
    virtual Function derivative() const = 0;

    //returns arithmetically simplified function - NOTE: returns new-ly allocated function
    const Function simplify() const;

    //virtual destructor
    virtual ~AbstractFunction() {
    };
};


#endif /* AbstractFunction_hpp */
