//
//  Unary.cpp
//  BabyWolf
//
//

#include "Unary.hpp"

Unary::Unary(std::string o, const Function* fn): fn(fn){
    op = stringToOperationType[o];
}

Unary::Unary(OperationType o, const Function* fn): op(o), fn(fn){}

Unary::Unary(const Unary& u){
    this->op = u.op;
    Function* f = u.fn->copy();
    this->fn = f;
}

Unary& Unary::operator=(Unary u){
    std::swap(*this, u);
    return *this;
}

Function* Unary::copy() const {
    return new Unary(*this);
}

double Unary::evaluate(double arg) const{
    double result = fn->evaluate(arg);
    switch (op){
        case NEG:
            return -1 * result;
        case INV:
            return 1 / result;
        case ABS:
            return (result > 0) ? result : (-1 * result);
        case LN:
            return log(result); //natural logarithm
        case LOG:
            return log(result)/log(opts.base); //log base b
        case SIN:
            return sin(result);
        case COS:
            return cos(result);
        case TAN:
            return tan(result);
        case SEC:
            return 1 / cos(result);
        case CSC:
            return 1 / sin(result);
        case COT:
            return 1 / tan(result);
        case ASIN:
            return asin(result);
        case ACOS:
            return acos(result);
        case ATAN:
            return atan(result);
        case ASEC:
            return acos(1/result);
        case ACSC:
            return asin(1/result);
        case ACOT:
            return atan(1/result);
        case SINH:
            return sinh(result);
        case COSH:
            return cosh(result);
        case TANH:
            return tanh(result);
        case SECH:
            return 1 / cosh(result);
        case CSCH:
            return 1 / sinh(result);
        case COTH:
            return 1 / tanh(result);
        case ASINH:
            return asinh(result);
        case ACOSH:
            return acosh(result);
        case ATANH:
            return atanh(result);
        case ASECH:
            return acosh(1/result);
        case ACSCH:
            return asinh(1/result);
        case ACOTH:
            return atanh(1/result);
        case INVALID:
            return 0;
        default:
            return 0;
    }
    
}

Function* Unary::substitute(const Function* subFn) const {
    return new Unary(op, fn->substitute(subFn));
}

Function* Unary::derivative() const {
    //still need to implement for all supported unary functions
    return fn->derivative();
}

const Function* Unary::wrap() const {
    const Function* wrapFn = fn->wrap();
    return new Unary(op, wrapFn);
}

const Function* Unary::flatten() const {
    const Function* flatFn = fn->flatten();
    //std::cout << "Before flattening " << *flatFn << std::endl; //debug
    if (flatFn->getType() == FunctionType::UNARY && flatFn->getOperation() == op){
        if (op == ABS){
            return flatFn;
        } //else is negative of negative, or reciprocal of reciprocal
        else if (op == NEG || op == INV){
            return flatFn->getFns().first;
        }
    }
    
    return new Unary(op, flatFn);
}

const Function* Unary::collapse() const {
    const Function* simpleFn = fn->collapse();
    
    if (simpleFn->getType() == FunctionType::CONSTANT){
        return new Constant(this->evaluate(0)); //value of argument doesn't matter
    }
    
    return new Unary(op, simpleFn);
}


std::string Unary::getPrefixString() const {
    std::string str = "";
    str += "(";
    str += operationToString[op];
    str += " ";
    str += fn->getPrefixString();
    str += ")";
    return str;
}

std::string Unary::getInfixString() const {
    std::string str = "";
    if (op == NEG){
        str += "(- ";
    } else if (op == INV){
        str += "(1 / ";
    } else if (op == ABS){
        str += "|";
    } else {
        str += "(";
        str += operationToString[op];
        str += " ";
    }
    str += fn->getInfixString();
    if (op == ABS){
        str+= "|";
    } else {
        str+= ")";
    }
    return str;
}

FunctionType Unary::getType() const {
    return FunctionType::UNARY;
}

OperationType Unary::getOperation() const {
    return op;
}

std::pair<const Function*, std::vector<const Function*>> Unary::getFns() const { return {fn, std::vector<const Function*>()};}

Unary::~Unary(){
    delete fn;
}
