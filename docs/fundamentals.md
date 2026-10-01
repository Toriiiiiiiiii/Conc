# Conc Fundamentals

## Concatenative Programming
Concatenative programming is a subset of the Functional Programming paradigm. Programs are constructed by *concatenating* function calls to produce an output. For example, the basic Conc program `34 35 +` is a concatenation of 3 functions:
 1. Pushing the integer value `34` to the stack
 2. Pushing the integer value `35` to the stack
 3. Adding the two integers at the top of the stack.

This concept is extended to the whole of Conc, with functions taking arguments off of the stack and pushing their return value on the top.

## Atomic Functions
Conc implements a number of *atomic* functions, which are built in to the language itself, instead of being defined in a program. These functions perform the most basic operations required to process data, such as stack manipulation and arithmetic operations.

Symbol/Name | Operation
------------|-----------
`<n>`       | Pushes any integer value `<n>` onto the stack
`<var>`     | Pushes the value of variable `<var>` onto the stack
`.`         | Prints out the integer at the top of the stack
`:`         | Duplicates the integer value on the top of the stack
`+`         | Adds the two integer values on the top of the stack
`-`         | Subtracts the two integer values on the top of the stack
`*`         | Multiplies the two integer values on the top of the stack
`/`         | Divides the two integer values on the top of the stack
`%`         | Computes the remainder of the division of the two integer values on the top of the stack.
`=`         | Tests if the integer values at the top of the stack are equal
`<`, `>`    | Tests if the numbers at the top of the stack are less than/greater than the other
