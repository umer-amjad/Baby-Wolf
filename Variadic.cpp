//
//  Variadic.cpp
//  BabyWolf
//
//

#include <algorithm>

#include "Variadic.hpp"

Variadic::Variadic(std::string o, std::vector<const Function*> fns) : fns(fns) {
    op = stringToOperationType[o];
}

Variadic::Variadic(OperationType o, std::vector<const Function*> fns) : op(o), fns(fns) {
}

Variadic::Variadic(const Variadic& v) {
    for (auto& fn : v.fns) {
        this->fns.emplace_back(fn->copy());
    }
    op = v.op;
}

Variadic& Variadic::operator=(Variadic v) {
    std::swap(*this, v);
    return *this;
}

Function* Variadic::copy() const {
    return new Variadic(*this);
}

double Variadic::evaluate(double arg) const {
    double result = fns[0]->evaluate(arg); //start with first
    int reverser = (int) (fns.size() - 3); //allows to reverse iterator
    if (op == POWER)
        result = fns[fns.size() - 1]->evaluate(arg);
    for (auto f = fns.begin() + 1; f != fns.end(); f++) { // loop from second onwards
        switch (op) {
            case PLUS:
                result += (*f)->evaluate(arg);
                break;
            case MINUS:
                result -= (*f)->evaluate(arg);
                break;
            case TIMES:
                result *= (*f)->evaluate(arg);
                break;
            case DIVIDE:
                result /= (*f)->evaluate(arg);
                break;
            case POWER:
                result = pow((*(f + reverser))->evaluate(arg), result);
                reverser -= 2;
                break;
            default:
                break;
        }
    }
    return result;
}

Function* Variadic::substitute(const Function* subFn) const {
    std::vector<const Function*> substitutedFns;
    for (auto& fn : fns) {
        substitutedFns.emplace_back(fn->substitute(subFn));
    }
    return new Variadic(op, substitutedFns);
}

Function* Variadic::derivative() const {
    //implemented for + and *
    //still need to implement for ^ operator
    std::vector<const Function*> derivedFns;
    int i = 0;
    for (auto& fn : fns) {
        Function* derivedInner = fn->derivative();
        if (op == TIMES) {
            std::vector<const Function*> restProduct = fns;
            restProduct.erase(restProduct.begin() + i);
            restProduct.emplace_back(derivedInner);
            derivedFns.emplace_back(new Variadic(TIMES, restProduct));
        } else if (op == PLUS) {
            derivedFns.emplace_back(fn->derivative());
        }
        ++i;
    }
    return new Variadic(PLUS, derivedFns);
}

const Function* Variadic::wrap() const {
    //std::cout << "Before wrapping: " << *this << " end\n"; DEBUG
    OperationType wrapOp = op;
    std::vector<const Function*> wrapFns;
    switch (op) {
        case MINUS:
        {
            wrapFns.emplace_back(fns[0]->wrap());
            wrapOp = PLUS;
            for (auto it = fns.begin() + 1; it != fns.end(); it++) {
                wrapFns.emplace_back(new Unary(NEG, (*it)->wrap()));
            }
            break;
        }
        case DIVIDE:
        {
            wrapFns.emplace_back(fns[0]->wrap());
            wrapOp = TIMES;
            for (auto it = fns.begin() + 1; it != fns.end(); it++) {
                wrapFns.emplace_back(new Unary(INV, (*it)->wrap()));
            }
            break;
        }
        case POWER:
        {
            if (fns.size() > 2) {
                wrapFns.emplace_back(fns[0]->wrap());
                std::vector<const Function*> restFns(fns.begin() + 1, fns.end());
                wrapFns.emplace_back((new Variadic(POWER, restFns))->wrap());
                break;
            }
            //else continue to default
        }
        default:
        {
            for (const auto& f : fns) {
                wrapFns.emplace_back(f->wrap());
            }
        }
    }
    return new Variadic(wrapOp, wrapFns);
}

const Function* Variadic::flatten() const {
    std::vector<const Function*> flatFns;
    for (const auto& fn : fns) {
        const Function* flatFn = fn->flatten();
        //std::cout << "Flattened token is " << *flatFn << std::endl; //debug
        if (flatFn->getType() == FunctionType::VARIADIC &&
                flatFn->getOperation() == op && op != POWER) { //don't flatten power towers
            for (auto innerF : flatFn->getFns().second) {
                flatFns.push_back(innerF);
            }
        } else {
            flatFns.push_back(flatFn);
        }
    }
    return new Variadic(op, flatFns);
}

const Function* Variadic::collapse() const {
    //std::cout << "Before collapsing: " << *this << " end\n"; DEBUG
    std::vector<const Function*> collapseFns;
    std::transform(fns.begin(), fns.end(), std::back_inserter(collapseFns),
            [](const Function * f) -> const Function*{
                return f->collapse();
            });

    if (op == POWER) {
        Function* powerFn = new Variadic(op, collapseFns);
        if (collapseFns[0]->getType() == CONSTANT && collapseFns[1]->getType() == CONSTANT) {
            double result = powerFn->evaluate(0); //argument doesn't matter
            delete powerFn;
            return new Constant(result);
        }
        return powerFn;
    }
    //sort can only be done for + and *, not power:
    std::stable_sort(collapseFns.begin(), collapseFns.end(),
            [](const Function* a, const Function * b) -> bool{
                return a->getType() < b->getType();
            });
    //std::cout << "Before collapsing 2: " << *(new Variadic(op, collapseFns)) << " end\n"; //DEBUG
    std::vector<const Function*> simpleFns;
    auto fn = collapseFns.begin();
    for (; fn != collapseFns.end(); fn++) {
        if ((*fn)->getType() == FunctionType::CONSTANT) {
            break;
        } else {
            simpleFns.emplace_back(*fn);
        }
    }
    if (fn != collapseFns.end()) {
        std::vector<const Function*> constFns(fn, collapseFns.end());
        double result = (new Variadic(op, constFns))->evaluate(0); //argument to evaluate doesn't matter
        if ((op == PLUS && result != 0) || (op == TIMES && result != 1)) {
            simpleFns.emplace_back(new Constant(result));
        }
        for (; fn != collapseFns.end(); fn++) {
            delete *fn;
        }
    }
    switch (simpleFns.size()) {
        case 0:
            return ((op == PLUS) ? new Constant(0) : new Constant(1)); //nothing was added because only arg was 0
            break;
        case 1:
            return simpleFns[0];
            break;
        default:
            return new Variadic(op, simpleFns);
            break;
    }
}

std::string Variadic::getPrefixString() const {
    std::string str = "";
    str += "(";
    str += operationToString[op];
    for (auto f : fns) {
        str += " ";
        str += f->getPrefixString();
    }
    str += ")";
    return str;
}

std::string Variadic::getInfixString() const {
    std::string str = "";
    str += "(";
    for (auto f : fns) {
        str += f->getInfixString();
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

std::pair<const Function*, std::vector<const Function*>> Variadic::getFns() const {
    return {nullptr, fns};
}

Variadic::~Variadic() {
    for (auto f : fns) {
        delete f;
        f = nullptr;
    }
    fns.clear();
}
