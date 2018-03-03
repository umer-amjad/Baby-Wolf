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
// parse wraps everything, calls absoluteValueSubstitution once, and calls parseToken once
// parseToken and tokenize are then mutually recursive
// absoluteValueSubstitution changes | symbol to < and > symbols, for opening and closing abs
//  and also does bracket symbols and absolute value symbols error checking

//returns pointer to function constructed from expr, nullptr if there is an error
const Function* parse(std::string expr);

// if error during parsing, returns {"", false}, otherwise returns
// the absolute value substituted string with true as second.
std::pair<std::string, bool> absoluteValueSubstitution(std::string expr);

// mutually recursive with tokenize; parses all expressions
const Function* parseToken(std::string expr);

// mutually recursive with parseToken; splits variadic expressions and calls parseToken on each
const Function* tokenize(std::string expr, char op);

#endif /* Parser_hpp */
