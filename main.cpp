//
//  main.cpp
//  BabyWolf
//
//

#include "Parser.hpp"

//see notes in Parser.hpp and Function.hpp

int main(int argc, const char * argv[]) {
    Function::opts = {true, false};
    const Function* subTest = parse("x^2+sin(cos(x))"); //testing substitution
    while (true) {
        double arg = 0;
        std::string myExpr;
        std::cout << "Enter function, q to quit, or o for display options: " << std::endl;
        getline(std::cin, myExpr);
        if (myExpr == "q"){
            break;
        } else if (myExpr == "o"){
            std::cout << "Enter 0 or 1 to toggle options.\n"
                << "True indicates that that display option will be used. Press d when done.\n";
            while(true){
                std::cout << std::boolalpha;
                std::cout << "0: Prefix: " << Function::opts.prefix << '\n';
                std::cout << "1: Infix: " << Function::opts.infix << std::endl;
                std::string displayOption;
                getline(std::cin, displayOption);
                if (displayOption == "d"){
                    break;
                } else if (displayOption == "0"){
                    Function::opts.prefix = !Function::opts.prefix;
                } else if (displayOption == "1"){
                    Function::opts.infix = !Function::opts.infix;
                } else {
                    std::cout << "Please enter d, 0, or 1\n";
                }
            }
            continue;
        }
        const Function* f(parse(myExpr));
        if (f == nullptr)
            continue;
        std::cout << "Function is: " << '\n';
        std::cout << *f;
        const Function* simpleF(f->simplify());
        std::cout << "Simplified function is: " << '\n';
        std::cout << *simpleF;
//        std::cout << "Substituted function using  is: x^2+sin(cos(x))" << '\n';
//        const Function* sub = f->substitute(subTest);
//        std::cout << *sub;
//        std::cout << "Original function is: " << '\n';
//        std::cout << "f(x) = " << *f << '\n';
        std::cout << "Enter argument: " << '\n';
        std::cin >> arg;
        std::cout << "f(" << arg << ") = " << simpleF->eval(arg) << '\n';
        std::cin.ignore();
     }
}

//test cases:
//1-2-3
//2^3^4
//2x^2+19x+2^(1+x/2)
//x^2 + 19x + 7 -1/2
// (2+5) + 6*(x^2 + 3^(45+x))
//(|x+3|)(1+5)|x+(2+9|3-x|)*(x|x-2|)|
//v
//(3 + (5 * (9 + (x + (3 * ((x ^ 2) * |(x + (5 * (13 + (|(x + 5)| * |(x - 5)| * (5 + (2 * |((x ^ 2) + 3)|))))) + (6 * (13 + (|(x + 5)| * |(x - 5)| * (5 + (2 * |((x ^ 2) + 3)|))))))|))))))
//1-2-x-3-(1+2-3-x-4-(7/5x/7/8))
//(x*1)+(x^2*2)+(x^2*1)+3x^2+4
//6+7+(8+(9-(-x)))
//x^(3+(7-5))^(x+2+3+4/2)^(-(-x))
//sinx+5
//cosx
//sin(x+5)
//acoshx
//acsc(x+3)
//asech(x^2+5x+3)



