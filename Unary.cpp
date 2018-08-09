//
//  Unary.cpp
//  BabyWolf
//
//

#include "Unary.hpp"
#include "Variadic.hpp"

Unary::Unary(std::string o, const Function* fn) : fn(fn) {
    op = stringToOperationType[o];
}

Unary::Unary(OperationType o, const Function* fn) : op(o), fn(fn) {
}

Unary::Unary(const Unary& u) {
    this->op = u.op;
    Function* f = u.fn->copy();
    this->fn = f;
}

Unary& Unary::operator=(Unary u) {
    std::swap(*this, u);
    return *this;
}

Function* Unary::copy() const {
    return new Unary(*this);
}

double Unary::evaluate(double arg) const {
    double result = fn->evaluate(arg);
    switch (op) {
        case NEG: return -1 * result;
        case INV: return 1 / result;
        case ABS: return std::abs(result);
        case LN: return log(result); //natural logarithm
        case LOG: return log(result) / log(opts.base); //log base b
        case SIN: return sin(result);
        case COS: return cos(result);
        case TAN: return tan(result);
        case SEC: return 1 / cos(result);
        case CSC: return 1 / sin(result);
        case COT: return 1 / tan(result);
        case ASIN: return asin(result);
        case ACOS: return acos(result);
        case ATAN: return atan(result);
        case ASEC: return acos(1 / result);
        case ACSC: return asin(1 / result);
        case ACOT: return atan(1 / result);
        case SINH: return sinh(result);
        case COSH: return cosh(result);
        case TANH: return tanh(result);
        case SECH: return 1 / cosh(result);
        case CSCH: return 1 / sinh(result);
        case COTH: return 1 / tanh(result);
        case ASINH: return asinh(result);
        case ACOSH: return acosh(result);
        case ATANH: return atanh(result);
        case ASECH: return acosh(1 / result);
        case ACSCH: return asinh(1 / result);
        case ACOTH: return atanh(1 / result);
        case INVALID: return 0;
        default: return 0;
    }
}

Function* Unary::substitute(const Function* subFn) const {
    return new Unary(op, fn->substitute(subFn));
}

Function* Unary::derivative() const {
    // unary is of the form h = g(f), so h' = f' * g'(f)
    // add f' to the chain rule
    std::vector<const Function*> chain{fn->derivative()};
    Function* gpf = nullptr; // this is g'(f) 
    switch (op) {
        case NEG: gpf = new Constant(-1);
            break;
        case INV: gpf = new Unary(NEG, new Variadic(POWER,{fn->copy, new Constant(2)}));
            break;
        case ABS: gpf = new Constant(1); // for now, if g = |f|, g' = f' * 1
            break;
        case LN: gpf = new Unary(INV, fn->copy()); //natural logarithm
            break;
        case LOG: gpf = new Variadic(TIMES,{new Constant(1 / log(opts.base)), new Unary(INV, fn->copy())});
            break;
        case SIN: gpf = new Unary(COS, fn->copy());
            break;
        case COS: gpf = new Unary(NEG, new Unary(SIN, fn->copy()));
            break;
        case TAN: gpf = new Variadic(POWER,{new Unary(SEC, fn->copy()), new Constant(2)});
            break;
        case ASIN: gpf = new Unary(INV, new Variadic(POWER,{new Variadic(PLUS,
                {new Constant(1),
                    new Unary(NEG, new Variadic(POWER,
                    {fn->copy(), new Constant(2)}))}), new Constant(1 / 2)}));
            break;
        case ACOS: gpf = new Unary(NEG, new Unary(INV, new Variadic(POWER,{new Variadic(PLUS,
                {new Constant(1),
                    new Unary(NEG, new Variadic(POWER,
                    {fn->copy(), new Constant(2)}))}), new Constant(1 / 2)})));
            break;
        case ATAN: gpf = new Unary(INV, new Variadic(PLUS,{new Constant(1), new Variadic(POWER,
                {fn->copy(), 2})}));
            break;
        case SINH:
        case COSH:
        case TANH:
        case ASINH:
        case ACOSH:
        case ATANH:
        //functions should not exist after wrapping
        case SEC: 
        case CSC:
        case COT:
        case ASEC: 
        case ACSC: 
        case ACOT: 
        case SECH: 
        case CSCH: 
        case COTH: 
        case ASECH: 
        case ACSCH: 
        case ACOTH: 
        case INVALID:
        default: return new Constant(0);
    }
    return fn->derivative();
}

const Function* Unary::wrap() const {
    const Function* wrapFn = fn->wrap();
    switch (op) {
        case SEC: return new Unary(INV, new Unary(COS, wrapFn));
        case CSC: return new Unary(INV, new Unary(SIN, wrapFn));
        case COT: return new Unary(INV, new Unary(TAN, wrapFn));
        case ASEC: return new Unary(ACOS, new Unary(INV, wrapFn));
        case ACSC: return new Unary(ASIN, new Unary(INV, wrapFn));
        case ACOT: return new Unary(ATAN, new Unary(INV, wrapFn));
        case SECH: return new Unary(INV, new Unary(COSH, wrapFn));
        case CSCH: return new Unary(INV, new Unary(SINH, wrapFn));
        case COTH: return new Unary(INV, new Unary(TANH, wrapFn));
        case ASECH: return new Unary(ACOSH, new Unary(INV, wrapFn));
        case ACSCH: return new Unary(ASINH, new Unary(INV, wrapFn));
        case ACOTH: return new Unary(ATANH, new Unary(INV, wrapFn));
        case INVALID: return new Constant(0);
        default: return new Unary(op, wrapFn);
    }
}

const Function* Unary::flatten() const {
    const Function* flatFn = fn->flatten();
    //std::cout << "Before flattening " << *flatFn << std::endl; //debug
    if (flatFn->getType() == FunctionType::UNARY && flatFn->getOperation() == op) {
        if (op == ABS) {
            return flatFn;
        }//else is negative of negative, or reciprocal of reciprocal
        else if (op == NEG || op == INV) {

            return flatFn->getFns().first;
        }
    }

    return new Unary(op, flatFn);
}

const Function* Unary::collapse() const {
    const Function* simpleFn = fn->collapse();

    if (simpleFn->getType() == FunctionType::CONSTANT) {

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
    if (op == NEG) {
        str += "(- ";
    } else if (op == INV) {
        str += "(1 / ";
    } else if (op == ABS) {
        str += "|";
    } else {
        str += "(";
        str += operationToString[op];
        str += " ";
    }
    str += fn->getInfixString();
    if (op == ABS) {
        str += "|";
    } else {

        str += ")";
    }
    return str;
}

FunctionType Unary::getType() const {

    return FunctionType::UNARY;
}

OperationType Unary::getOperation() const {

    return op;
}

std::pair<const Function*, std::vector<const Function*>> Unary::getFns() const {

    return{fn, std::vector<const Function*>()};
}

Unary::~Unary() {
    delete fn;
}
