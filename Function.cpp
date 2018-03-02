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
    const Function* simplified = this->wrap()->flatten()->collapse()->flatten();
    user_functions.push_back(simplified);
    return simplified;
}

std::pair<const Function*, std::vector<const Function*>> Function::getFns() const {
    return {nullptr, std::vector<const Function*>()};
}

std::ostream& operator<<(std::ostream& o, const Function& fn){
    auto iter = std::find(Function::user_functions.begin(), Function::user_functions.end(), &fn);
    int index = (int) (iter - Function::user_functions.begin());
    if (iter == Function::user_functions.end()){
        index = -1;
    }
    if (Function::opts.prefix){
        o << "(define (f{" << index << "}" << " x) " << fn.getPrefixString() << ")\n";
    }
    if (Function::opts.infix){
        o << "f{" << index << "}" << "(x) = " << fn.getInfixString() << '\n';
    }
    return o;
}
