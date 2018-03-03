//
//  Parser.hpp
//  BabyWolf
//
//

#ifndef Parser_hpp
#define Parser_hpp

#include <cctype>
#include <algorithm>

#include "Argument.hpp"
#include "Constant.hpp"
#include "Unary.hpp"
#include "Variadic.hpp"

// overall structure:
// parse wraps everything, calls cleanAbsolutes once, and calls parseToken once
// bracketCheck makes sure the input has a correct bracket structure
// parseToken and tokenize are then mutually recursive
// cleanAbsolutes is called during parseToken, if the token is surrounded by brackets
// to add multiplication symbols between absolute value symbols if needed

const Function* parse(std::string expr);

bool bracketCheck(std::string expr);

std::string cleanAbsolutes(std::string expr);

std::pair<std::string, bool> cleanAbsolutes2(std::string expr);

const Function* parseToken(std::string expr);

const Function* tokenize(std::string expr, char op);

#endif /* Parser_hpp */
