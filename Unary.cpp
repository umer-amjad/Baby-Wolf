//
//  Unary.cpp
//  BabyWolf
//
//

#include "Unary.hpp"
#include "Variadic.hpp"

Unary::Unary(std::string o, const AbstractFunction* fn) : fn(fn) {
    op = stringToOperationType[o];
}

Unary::Unary(OperationType o, const AbstractFunction* fn) : op(o), fn(fn) {
}

Unary::Unary(const Unary& u) {
    this->op = u.op;
    AbstractFunction* f = u.fn->copy();
    this->fn = f;
}

Unary& Unary::operator=(Unary u) {
    std::swap(*this, u);
    return *this;
}

AbstractFunction* Unary::copy() const {
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

AbstractFunction* Unary::substitute(const AbstractFunction* subFn) const {
    return new Unary(op, fn->substitute(subFn));
}

AbstractFunction* Unary::derivative() const {
    // unary is of the form h = g(f), so h' = f' * g'(f)
    // add f' to the chain rule
    AbstractFunction* fp = fn->derivative();
    AbstractFunction* gpf = nullptr; // this is g'(f) 
    switch (op) {
        case LN: return new Variadic(TIMES, {new Unary(INV, fn->copy()), fp}); //natural logarithm
        case NEG: return new Unary(NEG, fp);
        case INV: return new Variadic(TIMES, {new Unary(NEG, new Variadic(POWER,{fn->copy(), new Constant(2)})), fp});
        case ABS: return fp; // for now, if g = |f|, g' = f'
        case LOG: return new Variadic(TIMES, {new Constant(1 / log(opts.base)), new Unary(INV, fn->copy())});
        case SIN: return new Variadic(TIMES, {new Unary(COS, fn->copy()), fp});
        case COS: return new Variadic(TIMES, {new Unary(NEG, new Unary(SIN, fn->copy())), fp});
        case TAN: return new Variadic(TIMES, {new Variadic(POWER,{new Unary(SEC, fn->copy()), new Constant(2)}), fp});
        case ASIN: return new Variadic(TIMES, {new Unary(INV, new Variadic(POWER,{new Variadic(PLUS,
                {new Constant(1),
                    new Unary(NEG, new Variadic(POWER,
                {fn->copy(), new Constant(2)}))}), new Constant(1 / 2)})), fp});
        case ACOS: return new Variadic(TIMES, {new Unary(NEG, new Unary(INV, new Variadic(POWER,{new Variadic(PLUS,
                {new Constant(1),
                    new Unary(NEG, new Variadic(POWER,
                    {fn->copy(), new Constant(2)}))}), new Constant(1 / 2)}))), fp});
        case ATAN: return new Variadic(TIMES, {new Unary(INV, new Variadic(PLUS, {new Constant(1), new Variadic(POWER,
                {fn->copy(), new Constant(2)})})), fp});
        case SINH: return new Variadic(TIMES, {new Unary(COSH, fn->copy()), fp});
        case COSH: return new Variadic(TIMES, {new Unary(SINH, fn->copy()), fp});
        case TANH: return new Variadic(TIMES, {new Variadic(POWER,{new Unary(SECH, fn->copy()), new Constant(2)}), fp});
        case ASINH: return new Variadic(TIMES, {new Unary(INV, new Variadic(POWER, {new Variadic(PLUS,
                {new Constant(1),
                 new Variadic(POWER,
                {fn->copy(), new Constant(2)})}), new Constant(1 / 2)})), fp});
        case ACOSH: return new Variadic(TIMES, {new Unary(INV, new Variadic(POWER,{new Variadic(PLUS,
                {new Variadic(POWER, {fn->copy(), new Constant(2)}),
                 new Unary(NEG, new Constant(1))}), new Constant(1 / 2)})), fp});
        case ATANH: return new Variadic(TIMES, {new Unary(INV, new Variadic(PLUS,{new Constant(1), new Unary(NEG, new Variadic(POWER,
                {fn->copy(), new Constant(2)}))})), fp});
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
}

const AbstractFunction* Unary::wrap() const {
    const AbstractFunction* wrapFn = fn->wrap();
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

const AbstractFunction* Unary::flatten() const {
    const AbstractFunction* flatFn = fn->flatten();
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

const AbstractFunction* Unary::collapse() const {
    const AbstractFunction* simpleFn = fn->collapse();

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

std::pair<const AbstractFunction*, std::vector<const AbstractFunction*>> Unary::getFns() const {

    return{fn, std::vector<const AbstractFunction*>()};
}

Unary::~Unary() {
    delete fn;
}
