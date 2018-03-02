//
//  Function.cpp
//  BabyWolf
//
//

#include "Function.hpp"

Options Function::opts{true, false};
std::vector<const Function*> Function::user_functions{};

const Function* Function::wrap() const {
    return this;
}

const Function* Function::flatten() const {
    return this;
}

char Function::getOp() const {
    return ' ';
}

const Function* Function::simplify() const {
    return this->wrap()->flatten()->collapse()->flatten();
};

std::pair<const Function*, std::vector<const Function*>> Function::getFns() const {
    return {nullptr, std::vector<const Function*>()};
}

std::ostream& operator<<(std::ostream& o, const Function& fn){
    if (Function::opts.prefix){
        o << "(define (f{" << Function::user_functions.size() - 1 << "}" << " x) " << fn.getPrefixString() << ")\n";
    }
    if (Function::opts.infix){
        o << "f{" << Function::user_functions.size() - 1 << "}" << "(x) = " << fn.getInfixString() << '\n';
    }
    return o;
}
