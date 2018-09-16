# Baby-Wolf
This project is my attempt to write a mathematical function parser and evaluator, with cooler planned features such as derivative calculation and graphing - a baby Wolfram Alpha or Desmos, hopefully. 

#### Background ####
The project is written entirely in C++ and currently uses only containers and features from the C++ Standard Library. I compile it using C++14, though the features used should allow for compiling with C++11.

#### Parsing ####
The parser currently supports the usual operations, +, -, *, /, and ^ (for exponentiation), absolute values (using vertical bars), and all trigonometric functions - `sin`, `cos`, `tan`, all their reciprocals, all their inverses, and all their hyperbolic analogues. 

It should be noted that the parser is built to work with human-written mathematical expressions (e.g. "x^2 +3(x+7)/sinx"), and is able to determine the meaning of the expressions using rules derived from the way humans interpret such expressions (e.g. order of operations, concatenation means multiplication, spaces are negligible, etc.) 

#### Evaluating ####
The evaluator can evaluate expressions that contain the above operations or functions. Currently, the evaluator and parser support only single variable functions. Derivative calculation has also been implemented for all supported functions, though derivative simplification is needed (more on that below).

#### Displaying ####
Currently all interaction (I/O) with the program is done in a command-line interface. The displayer outputs the parsed function to the CLI. Currently, there are two options for display - infix and prefix. "Infix" displays the result with the arithmetic operations, +, *, -, /, and ^, between their arguments, as is commonly written. The prefix option prints in Polish notation, representing the s-expression formed by the parsing of the function.   

#### Graphing ####
Graphing is being implemented as another display option. This is done currently using Xlib. Improvements are still needed for graph labelling. Here are some examples of Baby Wolf graphing functions:

`cos(x)`:
![](Samples/cos(x).png)

`tan(x)`:
![](Samples/tan(x).png)

`sin(x^2)+x/5`:
![](Samples/sin(x2)plusxover5.png)


#### Simplifying ####
Currently, a rudimentary simplifier has been implemented. It will simplify constant expressions as well as flatten the tree for repeated operations. For example, `((x+3)+3)+3)` would be simplified to `x+9`. Stronger simplification algorithms are planned, as they are necessary for the final step of the derivative calculation, for the derivative to be simplified into a more readable form. 

### Future ###
Features to be added recently include: 
- Better parsing for trigonometric functions when they are concatenated with each other and other expressions
- Display LaTeX string of inputted function
- ~~Allow user defined functions to be used in future function definitions, allowing for function substitution within a user defined function, e.g. "f_1(x) = x+3", "f_2(x) = x(x+1)*f_1(x^2)"~~ (may need improvement)
- ~~Generate a graph of the inputted function~~ (To be improved)
- Multivariable function support
- Better error checking conditions done during function evaluation, in case parser fails or evaluator fails or is invalid (i.e. input is out of the domain of the function), as well as messages to the user on why it failed (in progress)
