//
//  AbstractFunction.cpp
//  BabyWolf
//
//

#include "AbstractFunction.hpp"

#include <iostream>

Options AbstractFunction::opts{true, false, false, true};
std::map<std::string, const AbstractFunction*> AbstractFunction::userFunctions{};

std::map<OperationType, std::string> AbstractFunction::operationToString;
std::map<std::string, OperationType> AbstractFunction::stringToOperationType;

std::string OperationTypeToString(OperationType o){
    switch(o){
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case TIMES:
            return "*";
        case DIVIDE:
            return "/";
        case POWER:
            return "^";
        case NEG:
            return "neg";
        case INV:
            return "inv";
        case ABS:
            return "abs";
        case LN:
            return "ln";
        case LOG:
            return "log";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case TAN:
            return "tan";
        case SEC:
            return "sec";
        case CSC:
            return "csc";
        case COT:
            return "cot";
        case ASIN:
            return "asin";
        case ACOS:
            return "acos";
        case ATAN:
            return "atan";
        case ASEC:
            return "asec";
        case ACSC:
            return "acsc";
        case ACOT:
            return "acot";
        case SINH:
            return "sinh";
        case COSH:
            return "cosh";
        case TANH:
            return "tanh";
        case SECH:
            return "sech";
        case CSCH:
            return "csch";
        case COTH:
            return "coth";
        case ASINH:
            return "asinh";
        case ACOSH:
            return "acosh";
        case ATANH:
            return "atanh";
        case ASECH:
            return "asech";
        case ACSCH:
            return "acsch";
        case ACOTH:
            return "acoth";
        case INVALID:
            return "";
    }
}

void AbstractFunction::initalizeOperationTypeMaps(){
    for (OperationType type = PLUS; type != INVALID; type = (OperationType)(type + 1)){
        std::string typeString = OperationTypeToString(type);
        operationToString[type] = typeString;
        stringToOperationType[typeString] = type;
    }
}

const AbstractFunction* AbstractFunction::wrap() const {
    return this;
}

const AbstractFunction* AbstractFunction::flatten() const {
    return this;
}

OperationType AbstractFunction::getOperation() const {
    return INVALID;
}

const AbstractFunction* AbstractFunction::simplify() const {
    const AbstractFunction* simplified = this->wrap()->flatten()->collapse()->flatten();
    //userFunctions.push_back(simplified);
    return simplified;
}

std::pair<const AbstractFunction*, std::vector<const AbstractFunction*>> AbstractFunction::getFns() const {
    return {nullptr, std::vector<const AbstractFunction*>()};
}

std::string AbstractFunction::getName() const {
    return functionName;
}

bool AbstractFunction::setName(std::string name){
    this->functionName = name;
    auto userFnIter = userFunctions.find(name);
    if (userFnIter != userFunctions.end()){
        userFunctions.erase(userFnIter);
        return true;
    }
    return false;
}


std::ostream& operator<<(std::ostream& o, const AbstractFunction& fn){
    std::string name = fn.functionName;
    if (name == ""){
        name = "unnamed";
    }
    if (AbstractFunction::opts.prefix){
        o << "(define (" << name << " x) " << fn.getPrefixString() << ")\n";
    }
    if (AbstractFunction::opts.infix){
        o << name << "(x) = " << fn.getInfixString() << '\n';
    }
    return o;
}
