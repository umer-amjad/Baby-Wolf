//
//  main.cpp
//  BabyWolf
//
//

#include "Parser.hpp"

//see notes in Parser.hpp and Function.hpp

int main(int argc, const char * argv[]) {
    while (true) {
        double arg = 0;
        std::string myExpr;
        std::cout << "Enter function, or q to quit: " << '\n';
        getline(std::cin, myExpr);
        if (myExpr == "q"){
            break;
        }
        std::unique_ptr<const Function> f(parse(myExpr));
        if (f == nullptr)
            continue;
        std::cout << "Function is: " << '\n';
        std::cout << "f(x) = " << *f << '\n';
        std::unique_ptr<const Function> simpleF(f->simplify());
        std::cout << "Simplified function is: " << '\n';
        std::cout << "f(x) = " << *simpleF << '\n';
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



