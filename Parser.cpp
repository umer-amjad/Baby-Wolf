//
//  Parser.cpp
//  BabyWolf
//
//

#include "Parser.hpp"

const std::vector<char> ops{'+', '-', '*', '/', '^'};
const std::vector<std::string> trigFns{"asinh", "acosh", "atanh", "asech", "acsch", "acoth","asin", "acos", "atan", "asec", "acsc", "acot", "sinh", "cosh", "tanh", "sech", "csch", "coth", "sin", "cos", "tan", "sec", "csc", "cot"};

const Function* parse(std::string expr){
    //remove spaces:
    expr.erase(std::remove_if(expr.begin(), expr.end(),
                              [](unsigned char c){
                                  return std::isspace(c);
                              }),
               expr.end());
    if (!bracketCheck(expr)) //doesn't pass bracket check
        return nullptr;
    int i = 0;
    while (true){
        if (i == expr.length() - 1)
            break;
        if ((('0' <= expr[i] && expr[i] <= '9') || expr[i] == ')' || expr[i] == 'x' || expr[i] == 'i' || expr[i] == 'e')
            && (expr[i+1] == 'x' || expr[i+1] == '(' || expr[i+1] == 'p' || expr[i+1] == 'e')){
            expr.insert(i+1, "*");
        }
        i++;
    }
    expr = cleanAbsolutes(expr);
    //std::cout << "Cleaned and now: " << expr << '\n'; //debug
    return parseToken(expr);
}

bool bracketCheck(std::string expr){
    int brackets = 0;
    int length = (int) expr.size();
    bool invalid = false;
    int err = 0;
    for (int i = 0; i < length; i++){
        if (expr[i] == '(')
            ++brackets;
        else if (expr[i] == ')')
            --brackets;
        if (brackets < 0){
            invalid = true;
            err = i;
            break;
        }
    }
    if (brackets > 0){
        invalid = true;
        err = length;
    }
    if (invalid){
        std::cout << "Parse error: check brackets at position " << err << '\n';
        return false;
    }
    return true;
}

std::string cleanAbsolutes(std::string expr){
    int i = 0;
    bool inAbs = false;
    int brackets = 0;
    while (true){
        //std::cout << "At i: " << i << " expr is " << expr << '\n'; //debug
        if (i >= expr.length() - 1)
            break;
        if (expr[i] == '(')
            brackets++;
        if (expr[i] == ')')
            brackets--;
        if (brackets != 0){
            i++;
            continue;
        }
        if (expr[i] == '|'){
            inAbs = !inAbs; //toggle
        }
        if (inAbs && expr[i+1] == '|'){
            i++;
            continue; //don't add * between inside arg and | char
        }
        if ((  (('0' <= expr[i] && expr[i] <= '9') || expr[i] == ')' || expr[i] == '|' || expr[i] == 'x' || expr[i] == 'i' || expr[i] == 'e')
             && (expr[i+1] == '|'))
            || (expr[i] == '|' && (expr[i+1] == '(' || expr[i+1] == 'x' || expr[i+1] == 'p' || expr[i+1] == 'e') && !inAbs)){
            expr.insert(i+1, "*");
        }
        i++;
    }
    // std::cout << "Cleaned and now: " << expr << '\n'; //debug
    return expr;
}

const Function* parseToken(std::string expr){
    //   std::cout << "Here 0 " << expr << '\n'; //debug
    int length = (int) expr.size();
    const Function* f = nullptr;
    if (length == 1){
        if (expr == "x"){
            f = new Argument;
            return f;
        }
    }
    std::vector<Function*> fns;
    
    //variadic operations:
    for (char op : ops){
        f = tokenize(expr, op);
        if (f != nullptr)
            return f;
    }
    
    // negatives
    if (expr[0] == '-'){
        return new Unary("neg", parseToken(expr.substr(1, length - 1)));
    }
    
    //absolute value
    if (expr[0] == '|'){
        return new Unary("abs", parseToken(expr.substr(1, length - 2)));
    }
    
    //trig functions, check longer ones before shorter ones
    for (auto trigFn : trigFns){
        int trigFnLength = (int) trigFn.length();
        if (length >= trigFnLength && expr.substr(0, trigFnLength) == trigFn){
            return new Unary(trigFn, parseToken(expr.substr(trigFnLength, length - trigFnLength)));
        }
    }
    
    //then brackets
    if (expr[0] == '('){
        return (parseToken(cleanAbsolutes(expr.substr(1, length - 2))));
    }
    if (expr == "pi"){
        return new Constant(M_PI);
    }
    if (expr == "e"){
        return new Constant(M_E);
    }
    //std::cout << "Here " << expr << '\n'; //debug
    f = new Constant(stod(expr));
    return f;
}

const Function* tokenize(std::string expr, char op){
    int length = (int) expr.size();
    const Function* f = nullptr;
    int brackets = 0;
    bool inAbs = false; //toggle between in and out of abs
    int substringStart = 0;
    int substringLength = 0;
    std::vector<const Function*> fns;
    for (int i = 0; i < length; i++){
        ++substringLength;
        if (expr[i] == '(')
            ++brackets;
        else if (expr[i] == ')')
            --brackets;
        else if (expr[i] == '|')
            inAbs = !inAbs;
        else if (brackets == 0 && (expr[i] == op && !inAbs && i > 0)){
            //length - 1 because don't take op char:
            fns.emplace_back(parseToken(expr.substr(substringStart, substringLength - 1)));
            substringStart = i + 1;
            substringLength = 0;
        }
    }
    if (substringStart > 0){
        fns.emplace_back(parseToken(expr.substr(substringStart, length - substringStart)));
        f = new Variadic(op, fns);
    }
    return f;
}

