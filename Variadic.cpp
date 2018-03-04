//
//  Variadic.cpp
//  BabyWolf
//
//

#include "Variadic.hpp"

Variadic::Variadic(char o,std::vector<const Function*> fns): op(o), fns(fns) {}

Variadic::Variadic(const Variadic& v){
    for (auto& fn: v.fns){
        this->fns.emplace_back(fn->copy());
    }
    op = v.op;
}

Variadic& Variadic::operator=(Variadic v){
    std::swap(*this, v);
    return *this;
}

Function* Variadic::copy() const {
    return new Variadic(*this);
}

double Variadic::eval(double arg) const{
    double result = fns[0]->eval(arg); //start with first
    int reverser = (int) (fns.size() - 3); //allows to reverse iterator
    if (op == '^')
        result = fns[fns.size() - 1]->eval(arg);
    for (auto f = fns.begin() + 1; f !=fns.end() ; f++){ // loop from second onwards
        switch (op) {
            case '+':
                result += (*f)->eval(arg);
                break;
            case '-':
                result -= (*f)->eval(arg);
                break;
            case '*':
                result *= (*f)->eval(arg);
                break;
            case '/':
                result /= (*f)->eval(arg);
                break;
            case '^':
                result = pow((*(f + reverser))->eval(arg), result);
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
    for (auto& fn: fns){
        substitutedFns.emplace_back(fn->substitute(subFn));
    }
    return new Variadic(op, substitutedFns);
}

const Function* Variadic::wrap() const {
    //std::cout << "Before wrapping: " << *this << " end\n"; DEBUG
    char wrapOp = op;
    std::vector<const Function*> wrapFns;
    if (op == '-'){
        wrapFns.emplace_back((*fns.begin())->wrap());
        auto it = fns.begin() + 1;
        wrapOp = '+';
        for (; it != fns.end(); it++){
            wrapFns.emplace_back(new Unary("neg", (*it)->wrap()));
        }
    } else if (op == '/'){
        wrapFns.emplace_back(*fns.begin());
        auto it = fns.begin() + 1;
        wrapOp = '*';
        for (; it != fns.end(); it++){
            wrapFns.emplace_back(new Unary("inv", (*it)->wrap()));
        }
    } else {
        for (auto f: fns){
            wrapFns.emplace_back(f->wrap());
        }
    }
    return new Variadic(wrapOp, wrapFns);
}

const Function* Variadic::flatten() const {
    std::vector<const Function*> flatFns;
    for (auto fn : fns){
        const Function* flatFn = fn->flatten();
        //std::cout << "Flattened token is " << *flatFn << std::endl; //debug
        if (flatFn->getType() == FunctionType::VARIADIC && flatFn->getOp() == op){
            for (auto innerF: flatFn->getFns().second){
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
                   [](const Function* f) -> const Function* {
                       return f->collapse();
                   });
    if (op == '^'){
        return new Variadic(op, collapseFns);
    }
    std::stable_sort(collapseFns.begin(), collapseFns.end(),
              [](const Function* a, const Function* b) -> bool{
                  return a->getType() < b->getType();
              });
    //std::cout << "Before collapsing 2: " << *(new Variadic(op, collapseFns)) << " end\n"; //DEBUG
    std::vector<const Function*> simpleFns;
    auto fn = collapseFns.begin();
    for (; fn != collapseFns.end(); fn++){
        if((*fn)->getType() == FunctionType::CONSTANT) {
            break;
        } else {
            simpleFns.emplace_back(*fn);
        }
    }
    if (fn != collapseFns.end()){
        std::vector<const Function*> constFns(fn, collapseFns.end());
        double result = (new Variadic(op, constFns))->eval(0); //argument to eval doesn't matter
        if ((op == '+' && result != 0) || (op == '*' && result != 1)){
            simpleFns.emplace_back(new Constant(result));
        }
        for (; fn != collapseFns.end(); fn++){
            delete *fn;
        }
    }
    switch (simpleFns.size()) {
        case 0:
            return ((op == '+') ? new Constant(0) : new Constant(1)); //nothing was added because only arg was 0
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
    str += op;
    for (auto f: fns){
        str += " ";
        str += f->getPrefixString();
    }
    str += ")";
    return str;
}

std::string Variadic::getInfixString() const {
    std::string str = "";
    str += "(";
    for (auto f: fns){
        str += f->getInfixString();
        str += " ";
        str += op;
        str += " ";
    }
    str.pop_back();
    str.pop_back();
    str.pop_back();
    str += ")";
    return str;
}

FunctionType Variadic::getType() const { return FunctionType::VARIADIC;}

char Variadic::getOp() const {return op;}

std::pair<const Function*, std::vector<const Function*>> Variadic::getFns() const {return {nullptr, fns};}

Variadic::~Variadic(){
    for (auto f : fns){
        delete f;
        f = nullptr;
    }
    fns.clear();
}
