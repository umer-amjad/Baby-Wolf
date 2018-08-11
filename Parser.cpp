//
//  Parser.cpp
//  BabyWolf
//
//

#include "Parser.hpp"

#include <map>
#include <iostream>

const std::vector<char> ops{'+', '-', '*', '/', '^'};
const std::vector<std::string> unaryFns{"asinh", "acosh", "atanh", "asech", "acsch", "acoth", "asin", "acos", "atan", "asec", "acsc", "acot", "sinh", "cosh", "tanh", "sech", "csch", "coth", "sin", "cos", "tan", "sec", "csc", "cot", "log", "ln"};

const AbstractFunction* parse(std::string expr) {
    //initialize operation maps:
    AbstractFunction::initalizeOperationTypeMaps();

    //remove spaces:
    bool hasName = false;
    std::string functionName;
    expr.erase(std::remove_if(expr.begin(), expr.end(),
            [](unsigned char c){
                return std::isspace(c);
            }),
    expr.end());
    auto equalSignPos = expr.find('=');
    if (equalSignPos != std::string::npos) {
        functionName = expr.substr(0, equalSignPos - 3);
        expr = expr.substr(equalSignPos + 1);
        hasName = true;
    } else {
        functionName = "f{" + std::to_string(AbstractFunction::userFunctions.size()) + "}";
    }
    std::pair < std::string, bool> exprValid = absoluteValueSubstitution(expr);
    if (!exprValid.second) {
        return nullptr;
    }

    int i = 0;
    expr = exprValid.first;
    while (true) {
        if (i == expr.length() - 1)
            break;
        if ((('0' <= expr[i] && expr[i] <= '9') || expr[i] == ')' || expr[i] == '>' || expr[i] == 'x' || expr[i] == 'i' || expr[i] == 'e')
                && (expr[i + 1] == 'x' || expr[i + 1] == '(' || expr[i + 1] == '<' || expr[i + 1] == 'p' || expr[i + 1] == 'e')) {
            expr.insert(i + 1, "*");
        }
        i++;
    }
    // std::cout << "Cleaned and now: " << expr << '\n'; //debug
    AbstractFunction* parsedFunction = parseToken(expr);
    parsedFunction->setName(functionName);
    // std::cout << "Function is " << *parsedFunction << std::endl; //debug
    AbstractFunction::userFunctions.insert({functionName, parsedFunction});
    return parsedFunction;
}

std::pair<std::string, bool> absoluteValueSubstitution(std::string expr) {
    int bracketLevel = 0;
    std::map<int, int> bracketToAbs;
    bracketToAbs[0] = 0;
    int i = 0;
    while (true) {
        if (i == expr.length()) {
            break;
        }
        char& symbol = expr[i];
        switch (symbol) {
            case '(':
                ++bracketLevel;
                bracketToAbs[bracketLevel] = 0;
                break;
            case ')':
                if (bracketLevel == 0) {
                    std::cerr << "Parse error at position " << i << ": too many closing parentheses" << std::endl;
                    return {"", false};
                }
                if (bracketToAbs[bracketLevel] != 0) {
                    std::cerr << "Parse error at position " << i << ": could not find closing absolute value symbol" << std::endl;
                    return {"", false};
                }
                --bracketLevel;
                break;
            case '|':
                if (bracketToAbs[bracketLevel] == 0) {
                    symbol = '<';
                    ++bracketToAbs[bracketLevel];
                    break;
                }
                if (i > 0 && std::find(ops.begin(), ops.end(), expr[i - 1]) != ops.end()) {
                    symbol = '<';
                    ++bracketToAbs[bracketLevel];
                    break;
                }
                if (bracketToAbs[bracketLevel] > 0) {
                    symbol = '>';
                    --bracketToAbs[bracketLevel];
                    break;
                }
                std::cerr << "Undefined behaviour at position " << i << ": contact Umer" << std::endl;
                return {"", false};
            default:
                break;
        }
        ++i;
    }
    if (bracketLevel > 0) {
        std::cerr << "Parse error at position " << i << ": missing closing parenthesis" << std::endl;
        return {"", false};
    }
    if (bracketLevel < 0) {
        std::cerr << "Parse error at position " << i << ": too many closing parentheses" << std::endl;
        return {"", false};
    }
    if (bracketToAbs[0] != 0) {
        std::cerr << "Parse error at position " << i << ": could not find closing absolute value symbol" << std::endl;
        return {"", false};
    }
    return {expr, true};
}

AbstractFunction* parseToken(std::string expr) {
    //   std::cout << "Here 0 " << expr << '\n'; //debug
    int length = (int) expr.size();
    AbstractFunction* f = nullptr;
    if (length == 1) {
        if (expr == "x") {
            f = new Argument;
            return f;
        }
    }
    std::vector<AbstractFunction*> fns;

    //variadic operations:
    for (char op : ops) {
        f = tokenize(expr, op);
        if (f != nullptr)
            return f;
    }

    // negatives
    if (expr[0] == '-') {
        return new Unary(NEG, parseToken(expr.substr(1)));
    }

    //absolute value
    if (expr[0] == '<') {
        return new Unary(ABS, parseToken(expr.substr(1, length - 2)));
    }

    //trig functions, check longer ones before shorter ones
    for (auto trigFn : unaryFns) {
        int trigFnLength = (int) trigFn.length();
        if (length >= trigFnLength && expr.substr(0, trigFnLength) == trigFn) {
            return new Unary(trigFn, parseToken(expr.substr(trigFnLength)));
        }
    }

    //user defined functions
    for (auto userFnIter = AbstractFunction::userFunctions.rbegin(); userFnIter != AbstractFunction::userFunctions.rend(); ++userFnIter) {
        //return must be first character, position 0
        if (expr.find(userFnIter->first) == 0) {
            int nameSize = (int) userFnIter->first.size();
            return userFnIter->second->substitute(parseToken(expr.substr(nameSize)));
        }
    }

    //then brackets
    if (expr[0] == '(') {
        return parseToken(expr.substr(1, length - 2));
    }
    if (expr == "pi") {
        return new Constant(M_PI);
    }
    if (expr == "e") {
        return new Constant(M_E);
    }
    //std::cout << "Here " << expr << '\n'; //debug
    f = new Constant(stod(expr));
    return f;
}

AbstractFunction* tokenize(std::string expr, char op) {
    int length = (int) expr.size();
    AbstractFunction* f = nullptr;
    int brackets = 0;
    int abs = 0;
    int substringStart = 0;
    int substringLength = 0;
    std::string operationString;
    operationString += op;
    std::vector<const AbstractFunction*> fns;
    for (int i = 0; i < length; i++) {
        ++substringLength;
        if (expr[i] == '(')
            ++brackets;
        else if (expr[i] == ')')
            --brackets;
        else if (expr[i] == '<')
            ++abs;
        else if (expr[i] == '>')
            --abs;
            //check i > 0 for if - is in front
        else if (brackets == 0 && abs == 0 && expr[i] == op && i > 0) {
            //length - 1 because don't take op char:
            fns.emplace_back(parseToken(expr.substr(substringStart, substringLength - 1)));
            substringStart = i + 1;
            substringLength = 0;
        }
    }
    if (substringStart > 0) {
        fns.emplace_back(parseToken(expr.substr(substringStart)));
        f = new Variadic(operationString, fns);
    }
    return f;
}

