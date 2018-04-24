//
//  Tests.hpp
//  Baby-Wolf
//
//
#ifndef Tests_hpp
#define Tests_hpp

#include <assert.h>
#include <iostream>

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
        assert(absoluteValueSubstitution
               ("x+|x+(x^2+|x^3+(x-|5+3|*3)-(x-|5+3|*3)*5|-5+(x-9|x+1|))+|x-1||+19").first ==
                "x+<x+(x^2+<x^3+(x-<5+3>*3)-(x-<5+3>*3)*5>-5+(x-9<x+1>))+<x-1>>+19");
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
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->evaluate(1.5) == 33.375);
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->evaluate(2) == 31);
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->evaluate(15) == 3640);
        assert(parse("x+|x+(x^2+|x^3+(x-|5+3|*3)+5|-5)|+19")->evaluate(-10.5) == 1290.375);
        assert(abs(parse("|x+|x^|x-|x/|x+3|-1|+2|-3|+4|")->evaluate(2) - 13.55606328618) < EPSILON);
        assert(abs(parse("(|x+3|)(1+5)|x+(2+9|3-x|)*(x|x-2|)|")->evaluate(-2.357) - 1998.50897889355) < EPSILON);
        assert(parse("3+(5*(9+(x+3(x^2(|x +5( 13 + |x+5||x-5|(5 + 2|x^2+3|))+6( 13 + |x+5||x-5|(5 + 2|x^2+3|))|)))))")->evaluate(5)
               == 55573);
        assert(parse("3+(5*(9+(x+3(x^2(|x +5( 13 + |x+5||x-5|(5 + 2|x^2+3|))+6( 13 + |x+5||x-5|(5 + 2|x^2+3|))|)))))")->evaluate(2)
               == 272098);
        assert(parse("3+(5*(9+(x+3(x^2(|x +5( 13 + |x+5||x-5|(5 + 2|x^2+3|))+6( 13 + |x+5||x-5|(5 + 2|x^2+3|))|)))))")->evaluate(-3)
               == 707973);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool testFunctionSubstitution(){
    try {
        double EPSILON = 0.000000001;
        assert(abs(parse("f(x) = sin(x)+cos(x)")->evaluate(0.5) - 1.3570081) < EPSILON);
        assert(abs(parse("h(x) = sin(x)+cos(x)")->evaluate(M_PI) - (-1)) < EPSILON);
        assert(abs(parse("yo(x) = f(x^2)")->evaluate(0.35) - 1.11470010531) < EPSILON);
        assert(abs(parse("yo(5)")->evaluate(0) - 0.858851061766) < EPSILON);
        assert(abs(parse("yo(5)")->evaluate(0.5839) - 0.858851061766) < EPSILON);
        assert(abs(parse("yo(5)")->evaluate(500) - 0.858851061766) < EPSILON);
        assert(abs(parse("hey(x) = f(x-1)+yo(x+1)")->evaluate(25) - (-1.85907157456)) < EPSILON);
        assert(abs(parse("hey(20)")->evaluate(500.39) - (2.44572244849)) < EPSILON);
        assert(abs(parse("hey(13.978)")->evaluate(29292.1) - (0.0768476566483)) < EPSILON);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool testAll(){
    if(testAbsoluteValueCleaner() && testAbsoluteValueEvaluator() && testFunctionSubstitution()){
        return true;
    }
    return false;
}

#endif /* Tests_hpp */
