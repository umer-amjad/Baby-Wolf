//
//  Unary.cpp
//  BabyWolf
//
//

#include "Unary.hpp"

Unary::Unary(std::string o, const Function* fn): op(o), fn(fn){}
double Unary::eval(double arg) const{
    double result = fn->eval(arg);
    if (op == "abs"){
        return (result > 0) ? result : (-1 * result);
    } else if (op == "neg"){
        return -1 * result;
    } else if (op == "inv"){
        return 1.0 / result;
    } else if (op == "sin"){
        return sin(result);
    } else if (op == "cos"){
        return cos(result);
    } else if (op == "tan"){
        return tan(result);
    } else if (op == "sec"){
        return 1.0 / cos(result);
    } else if (op == "csc"){
        return 1.0 / sin(result);
    } else if (op == "cot"){
        return 1.0 / tan(result);
    }
    return 0;
}

const Function* Unary::wrap() const {
    const Function * wrapFn = fn->wrap();
    return new Unary(op, wrapFn);
}

const Function* Unary::flatten() const {
    const Function * flatFn = fn->flatten();
    //std::cout << "Before flattening " << *flatFn << std::endl; //debug
    if (flatFn->getType() == FunctionType::UNARY && flatFn->getOp() == op[0]){
        if (op == "abs"){
            return flatFn;
        } //else is negative of negative, or reciprocal of reciprocal
        else if (op == "neg" || op == "inv"){
            return flatFn->getFns().first;
        }
    }
    
    return new Unary(op, flatFn);
}

const Function* Unary::collapse() const {
    const Function * simpleFn = fn->collapse();
    
    if (simpleFn->getType() == FunctionType::CONSTANT){
        return new Constant(this->eval(0)); //value of argument doesn't matter
    }
    
    return new Unary(op, simpleFn);
}


std::string Unary::getPrefixString() const {
    std::string str = "";
    str += "(";
    str += op;
    str += " ";
    str += fn->getPrefixString();
    str += ")";
    return str;
}

std::string Unary::getInfixString() const {
    std::string str = "";
    if (op == "neg"){
        str += "(- ";
    } else if (op == "inv"){
        str += "(1 / ";
    } else if (op == "abs"){
        str += "|";
    } else {
        str += "(";
        str += op;
        str += " ";
    }
    str += fn->getInfixString();
    if (op == "abs"){
        str+= "|";
    } else {
        str+= ")";
    }
    return str;
}

FunctionType Unary::getType() const { return FunctionType::UNARY;}

char Unary::getOp() const {return op[0];}

std::pair<const Function*, std::vector<const Function*>> Unary::getFns() const { return {fn, std::vector<const Function*>()};}

Unary::~Unary(){
    delete fn;
}
