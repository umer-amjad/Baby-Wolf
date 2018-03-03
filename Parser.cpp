//
//  Parser.cpp
//  BabyWolf
//
//

#include <map>

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
    int i = 0;
    std::pair<std::string, bool> expr_valid = cleanAbsolutes2(expr);
    if (!expr_valid.second){
        return nullptr;
    }
    expr = expr_valid.first;
    while (true){
        if (i == expr.length() - 1)
            break;
        if ((('0' <= expr[i] && expr[i] <= '9') || expr[i] == ')' || expr[i] == '>' || expr[i] == 'x' || expr[i] == 'i' || expr[i] == 'e')
            && (expr[i+1] == 'x' || expr[i+1] == '(' || expr[i+1] == '<' || expr[i+1] == 'p' || expr[i+1] == 'e')){
            expr.insert(i+1, "*");
        }
        i++;
    }
    //std::cout << "Cleaned and now: " << expr << '\n'; //debug
    const Function* parsedFunction = parseToken(expr);
    Function::user_functions.push_back(parsedFunction);
    return parsedFunction;
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

std::pair<std::string, bool> cleanAbsolutes2(std::string expr){
    int bracketLevel = 0;
    std::map<int, int> bracketToAbs;
    bracketToAbs[0] = 0;
    int i = 0;
    while(true){
        if (i == expr.length()){
            break;
        }
        if (bracketLevel < 0){
            std::cerr << "Parse error at position " << i << ": too many closing parentheses" << std::endl;
        }
        char& symbol = expr[i];
        switch (symbol){
            case '(':
                ++bracketLevel;
                bracketToAbs[bracketLevel] = 0;
                break;
            case ')':
                if (bracketToAbs[bracketLevel] != 0){
                    std::cerr << "Parse error at position " << i << ": could not find closing absolute value symbol" << std::endl;
                    return {"", false};
                }
                --bracketLevel;
                break;
            case '|':
                if (bracketToAbs[bracketLevel] == 0){
                    symbol = '<';
                    ++bracketToAbs[bracketLevel];
                    break;
                }
                if (i > 0 && std::find(ops.begin(), ops.end(), expr[i-1]) != ops.end()){
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
    if (bracketLevel != 0){
        std::cerr << "Parse error at position " << i << ": missing closing parenthesis" << std::endl;
    }
    return {expr, true};
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
    if (expr[0] == '<'){
        return new Unary("abs", parseToken(expr.substr(1, length - 2)));
    }
    
    //trig functions, check longer ones before shorter ones
    for (auto trigFn : trigFns){
        int trigFnLength = (int) trigFn.length();
        if (length >= trigFnLength && expr.substr(0, trigFnLength) == trigFn){
            return new Unary(trigFn, parseToken(expr.substr(trigFnLength, length - trigFnLength)));
        }
    }
    
    //user defined functions
    if (length >= 2 && expr.substr(0, 2) == "f{"){
        size_t digits = 0;
        int i = std::stoi(expr.substr(2), &digits);
        if (i < Function::user_functions.size()){
            return Function::user_functions[i]->substitute(parseToken(expr.substr(3+digits)));
        }
        else {
            std::cerr << "Index for user defined function too large: Function " << i << " not yet defined" << std::endl;
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
    int abs = 0;
    int substringStart = 0;
    int substringLength = 0;
    std::vector<const Function*> fns;
    for (int i = 0; i < length; i++){
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
        else if (brackets == 0 && abs == 0 && expr[i] == op && i > 0){
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

