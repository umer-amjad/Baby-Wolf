//
//  Function.cpp
//  BabyWolf
//
//

#include "Function.hpp"

Options Function::opts{true, false, false, true};
std::map<std::string, const Function*> Function::userFunctions{};

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
    //userFunctions.push_back(simplified);
    return simplified;
}

std::pair<const Function*, std::vector<const Function*>> Function::getFns() const {
    return {nullptr, std::vector<const Function*>()};
}

std::string Function::getName() const {
    return functionName;
}

bool Function::setName(std::string name){
    this->functionName = name;
    auto userFnIter = userFunctions.find(name);
    if (userFnIter != userFunctions.end()){
        userFunctions.erase(userFnIter);
        return true;
    }
    return false;
}


std::ostream& operator<<(std::ostream& o, const Function& fn){
//    auto iter = std::find(Function::userFunctions.begin(), Function::userFunctions.end(), &fn);
//    int index = (int) (iter - Function::userFunctions.begin());
//    if (iter == Function::userFunctions.end()){
//        index = -1;
//    }
    std::string name = fn.functionName;
    if (name == ""){
        name = "unnamed";
    }
    if (Function::opts.prefix){
        o << "(define (" << name << " x) " << fn.getPrefixString() << ")\n";
    }
    if (Function::opts.infix){
        o << name << "(x) = " << fn.getInfixString() << '\n';
    }
    return o;
}
