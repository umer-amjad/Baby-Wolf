//
//  Tests.hpp
//  Baby-Wolf
//
//
#ifndef Tests_hpp
#define Tests_hpp

#include <assert.h>

#include "Parser.hpp"

bool testAbsoluteValueCleaner(){
    try {
        assert(absoluteValueSubstitution("|x+|x+5|+19|").first == "<x+<x+5>+19>");
        assert(absoluteValueSubstitution("|x+|x^|x-|x/|x+3|||||").first == "<x+<x^<x-<x/<x+3>>>>>");
        assert(absoluteValueSubstitution("|x+|x^|x-|x/|x+3|-1|+2|-3|+4|").first == "<x+<x^<x-<x/<x+3>-1>+2>-3>+4>");
        assert(absoluteValueSubstitution("|x|x+5|x|").first == "<x>x+5<x>");
        assert(absoluteValueSubstitution("|x+(3x^2+|x+5|+17)+19x|").first == "<x+(3x^2+<x+5>+17)+19x>");
        assert(absoluteValueSubstitution("|x+(3x^2+|x+|x^|x-|x/|x+3|-1|+2|-3|+4|+17)+19x|").first == "<x+(3x^2+<x+<x^<x-<x/<x+3>-1>+2>-3>+4>+17)+19x>");
        assert(absoluteValueSubstitution("x+|x+5|+19").first == "x+<x+5>+19");
        assert(absoluteValueSubstitution("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19").first == "x+<x+(x^2+<x^3+(x-<5+3>*3)+5>-5)>+19");
        assert(absoluteValueSubstitution("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)+|3x+5|+2|+19").first == "x+<x+(x^2+<x^3+(x-<5+3>*3)+5>-5)+<3x+5>+2>+19");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool testAbsoluteValueEvaluator(){
    try {
        double EPSILON = 0.000000001;
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->eval(1.5) == 33.375);
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->eval(2) == 31);
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->eval(15) == 3640);
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->eval(-10.5) == 1290.375);
        assert(abs(parse("|x+|x^|x-|x/|x+3|-1|+2|-3|+4|")->eval(2) - 13.55606328618) < EPSILON);
//        std::cout << std::setprecision(15) << parse("(|x+3|)(1+5)|x+(2+9|3-x|)*(x|x-2|)|")->eval(-2.357) << std::endl;
        assert(abs(parse("(|x+3|)(1+5)|x+(2+9|3-x|)*(x|x-2|)|")->eval(-2.357) - 1998.50897889355) < EPSILON);
        assert(parse("3+(5*(9+(x+3(x^2(|x +5( 13 + |x+5||x-5|(5 + 2|x^2+3|))+6( 13 + |x+5||x-5|(5 + 2|x^2+3|))|)))))")->eval(5)
               == 55573);
        assert(parse("3+(5*(9+(x+3(x^2(|x +5( 13 + |x+5||x-5|(5 + 2|x^2+3|))+6( 13 + |x+5||x-5|(5 + 2|x^2+3|))|)))))")->eval(2)
               == 272098);
        assert(parse("3+(5*(9+(x+3(x^2(|x +5( 13 + |x+5||x-5|(5 + 2|x^2+3|))+6( 13 + |x+5||x-5|(5 + 2|x^2+3|))|)))))")->eval(-3)
               == 707973);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool testAll(){
    if(testAbsoluteValueCleaner() && testAbsoluteValueEvaluator()){
        return true;
    }
    return false;
}

#endif /* Tests_hpp */
