//
//  AbstractFunction.cpp
//  BabyWolf
//
//

#include "AbstractFunction.hpp"
#include "Function.hpp"

#include <iostream>

Options AbstractFunction::opts{true, false, false, true};
std::map<std::string, const Function> AbstractFunction::userFunctions{};

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

OperationType AbstractFunction::getOperation() const {
    return INVALID;
}

std::pair<const Function, std::vector<Function>> AbstractFunction::getFns() const {
    return {nullptr, std::vector< Function>()};
}
