//
//  Variadic.cpp
//  BabyWolf
//
//

#include <algorithm>

#include "Variadic.hpp"

Variadic::Variadic(std::string o, std::vector<Function> fns) : fns(fns) {
    op = stringToOperationType[o];
}

Variadic::Variadic(OperationType o, std::vector<Function> fns) : op(o), fns(fns) {
}

AbstractFunction* Variadic::copy() const {
    return new Variadic(op, fns);
}

double Variadic::evaluate(double arg) const {
    double result = fns[0].evaluate(arg); //start with first
    int reverser = (int) (fns.size() - 3); //allows to reverse iterator
    if (op == POWER)
        result = fns[fns.size() - 1].evaluate(arg);
    for (auto f = fns.begin() + 1; f != fns.end(); f++) { // loop from second onwards
        switch (op) {
            case PLUS:
                result += f->evaluate(arg);
                break;
            case MINUS:
                result -= f->evaluate(arg);
                break;
            case TIMES:
                result *= f->evaluate(arg);
                break;
            case DIVIDE:
                result /= f->evaluate(arg);
                break;
            case POWER:
                result = pow((f + reverser)->evaluate(arg), result);
                reverser -= 2;
                break;
            default:
                break;
        }
    }
    return result;
}

Function Variadic::substitute(const Function subFn) const {
    std::vector<Function> substitutedFns;
    for (auto& fn : fns) {
        substitutedFns.emplace_back(fn.substitute(subFn));
    }
    return Function(op, substitutedFns);
}

Function Variadic::derivative() const {
    std::vector<Function> derivedFns;
    // if op is ^
    if (op == POWER) {
        //must have exactly two fns, f^g
        Function f = fns[0];
        Function g = fns[1];
        // derivative is f^g * (g*(1/f)*f' + g' ln |f|
        Function f_prime = f.derivative();
        Function g_prime = g.derivative();
        Function f_inv(INV, f);
        Function abs_f(ABS, f);
        Function ln_abs_f(LN, abs_f);
        return Function(TIMES, 
                {Function(POWER, {f, g}), 
                Function(PLUS, 
                    {Function(TIMES, {g, f_inv, f_prime}),
                    Function(TIMES, {g_prime, ln_abs_f})})});
    }
    // else OP is + or *
    int i = 0;
    for (auto& fn : fns) {
        Function derivedInner = fn.derivative();
        if (op == TIMES) {
            std::vector<Function> restProduct = fns;
            restProduct.erase(restProduct.begin() + i);
            restProduct.emplace_back(derivedInner);
            derivedFns.emplace_back(Function(TIMES, restProduct));
        } else if (op == PLUS) {
            derivedFns.emplace_back(fn.derivative());
        }
        ++i;
    }
    return Function(PLUS, derivedFns);
}

const Function Variadic::wrap() const {
    OperationType wrapOp = op;
    std::vector<Function> wrapFns;
    switch (op) {
        case MINUS:
        {
            wrapFns.emplace_back(fns[0].wrap());
            wrapOp = PLUS;
            for (auto it = fns.begin() + 1; it != fns.end(); it++) {
                wrapFns.emplace_back(Function(NEG, it->wrap()));
            }
            break;
        }
        case DIVIDE:
        {
            wrapFns.emplace_back(fns[0].wrap());
            wrapOp = TIMES;
            for (auto it = fns.begin() + 1; it != fns.end(); it++) {
                wrapFns.emplace_back(Function(INV, it->wrap()));
            }
            break;
        }
        case POWER:
        {
            if (fns.size() > 2) {
                wrapFns.emplace_back(fns[0].wrap());
                std::vector<Function> restFns(fns.begin() + 1, fns.end());
                wrapFns.emplace_back((Function(POWER, restFns)).wrap());
                break;
            }
            //else continue to default
        }
        default:
        {
            for (const auto& f : fns) {
                wrapFns.emplace_back(f.wrap());
            }
        }
    }
    return Function(wrapOp, wrapFns);
}

const Function Variadic::flatten() const {
    std::vector<Function> flatFns;
    for (const auto& fn : fns) {
        const Function flatFn = fn.flatten();
        //std::cout << "Flattened token is " << *flatFn << std::endl; //debug
        if (flatFn.getType() == FunctionType::VARIADIC &&
                flatFn.getOperation() == op && op != POWER) { //don't flatten power towers
            for (const auto& innerF : flatFn.getFns().second) {
                flatFns.push_back(innerF);
            }
        } else {
            flatFns.push_back(flatFn);
        }
    }
    return Function(op, flatFns);
}

const Function Variadic::collapse() const {
    std::vector<Function> collapseFns;
    std::transform(fns.begin(), fns.end(), std::back_inserter(collapseFns),
            [](const Function& f) -> const Function {
                return f.collapse();
            });

    if (op == POWER) {
        Function powerFn(op, collapseFns);
        if (collapseFns[0].getType() == CONSTANT && collapseFns[1].getType() == CONSTANT) {
            double result = powerFn.evaluate(0); //argument doesn't matter
            return Function(result);
        }
        return powerFn;
    }
    //sort can only be done for + and *, not power:
    std::stable_sort(collapseFns.begin(), collapseFns.end());
    std::vector<Function> simpleFns;
    auto fn = collapseFns.begin();
    for (; fn != collapseFns.end(); fn++) {
        if (fn->getType() == FunctionType::CONSTANT) {
            break;
        } else {
            simpleFns.emplace_back(*fn);
        }
    }
    if (fn != collapseFns.end()) {
        std::vector<Function> constFns(fn, collapseFns.end());
        double result = Function(op, constFns).evaluate(0); //argument to evaluate doesn't matter
        if ((op == PLUS && result != 0) || (op == TIMES && result != 1)) {
            simpleFns.emplace_back(Function(result));
        }
        if (op == TIMES && result == 0){
        // return 0
            return Function(0.0);
        }
    }
    switch (simpleFns.size()) {
        case 0:
            return ((op == PLUS) ? Function(0.0) : Function(1.0)); //nothing was added because only arg was 0
            break;
        case 1:
            return simpleFns[0];
            break;
        default:
            return Function(op, simpleFns);
            break;
    }
}

std::string Variadic::getPrefixString() const {
    std::string str = "";
    str += "(";
    str += operationToString[op];
    for (const auto& f : fns) {
        str += " ";
        str += f.getPrefixString();
    }
    str += ")";
    return str;
}

std::string Variadic::getInfixString() const {
    std::string str = "";
    str += "(";
    for (const auto& f : fns) {
        str += f.getInfixString();
        str += " ";
        str += operationToString[op];
        str += " ";
    }
    str.pop_back();
    str.pop_back();
    str.pop_back();
    str += ")";
    return str;
}

FunctionType Variadic::getType() const {
    return FunctionType::VARIADIC;
}

OperationType Variadic::getOperation() const {
    return op;
}

std::pair<const Function, std::vector<Function>> Variadic::getFns() const {
    return {nullptr, fns};
}
