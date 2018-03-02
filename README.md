# Baby-Wolf
This project is my attempt to write a mathematical function parser and evaluator, with cooler planned features such as derivative calculation and graphing - a baby Wolfram Alpha or Desmos, hopefully. 

#### Background ####
The project is written entirely in C++ and currently uses only containers and features from the C++ Standard Library. I compile it using C++14, though the features used should allow for compiling with C++11.

#### Parsing ####
The parser currently supports the usual operations, +, -, *, /, and ^ (for exponentiation), absolute values (using vertical bars), and the three common trigonometric functions (sin, cos, and tan) and their reciprocals. 

It should be noted that the parser is built to work with human-written mathematical expressions (e.g. "x^2 +3(x+7)/sinx"), and is able to determine the meaning of the expressions using rules derived from the way humans interpret such expressions (e.g. order of operations, concatenation means multiplication, spaces are negligible, etc.) 

#### Evaluating ####
The evaluator can evaluate expressions that contain the above operations or functions. Currently, the evaluator and parser support only single variable functions.

#### Displaying ####
Currently all interaction (I/O) with the program is done in a command-line interface. The displayer outputs the parsed function to the CLI. Currently, there are two options for display - infix and prefix. "Infix" displays the result with the arithmetic operations, +, *, -, /, and ^, between their arguments, as is commonly written. The prefix option prints in Polish notation, representing the s-expression formed by the parsing of the function.   

#### Simplifying ####
Currently, a rudimentary simplifier has been implemented which has very limited functionality. 

### Future ###
Features to be added recently include: 
- Support parsing and evaluating: 
  - ~~inverses of the six trig functions~~
  - ~~hyperbolic trig functions~~
  - ~~inverses to the hyperbolic trig functions~~
  - logarithms
- Display LaTeX string of inputted function
- Parse absolute valued expressions in a smarter way, in particular recognizing and differentiating the "starting" vertical bar and the "ending" vertical bar
- ~~Allow user defined functions to be used in future function definitions, allowing for function substitution within a user defined function, e.g. "f_1(x) = x+3", "f_2(x) = x(x+1)*f_1(x^2)"~~
- ~~Improve the interface and add option for changing string output type in CLI~~


Features planned for further down the line are:
- Find the derivative of an input function and evaluate values of the derivative at some point
- Generate a graph of the inputted function 
- Multivariable function support
- Better error checking conditions done during function evaluation, in case parser fails or evaluator fails or is invalid (i.e. input is out of the domain of the function), as well as messages to the user on why it failed
