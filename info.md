# Pseudo code interpreter
## Info
This is a fully interpreted complete and non static typed pseudo language. The interpreter is written in C++ for improved performance over other high level languages

## Comments
Comments are completely discarded by the interpreter. A comment is every character following the `#` character.
```
# this is a comment
v <- 5 # this is another comment
# this is a
# multi-line
# comment
```

## Variable
A variable is the container of a value. A value type is one of the types following this paragraph
```
# declaring a variable
a <- 5

# using a variable
b <- a + 1  # if "b" wasn't previously declared this counts as declaration of "b"
```
Variable names can start with `_` and capital/lower case letters. Following allowed characters are `_`, capital/lower letters and numbers

## Types
### Number
A variable could contain a number. A number can be an integer or a double value
```
# int declaration
variable <- 5

write variable
```
output: `5`
```
# double declaration
variable <- 3.25

write variable
```
output: `3.25`

### String
A variable could contain a string. A string literal is a value enclosed in `"` characters
```
# string declaration
variable <- "this is a string"

write variable
```
output: `this is a string`

### Array
An array is a list of generic elements zero-indexed. This language is not statically typed so an array can contain elements of
heterogeneous types. To declare an array variable there are 2 ways
```
# empty array (value provided is discarded)
array[BEGIN] <- ""

write array
```
output: `[]`
```
# copy value array
array[3] <- "value provided"

write array
```
output: `['value provided', 'value provided', 'value provided', 'value provided']`
```
# array with heterogeneous types
list[1] <- 3.54
array[3] <- "value provided"
array[1] <- 5
array[2] <- list

write array
```
output: `['value provided', 5, [3.54, 3.54], 'value provided']`

## Operators
### Math operators
#### Operator `+`
- `number` + `number` returns a `number` (binary addition)
- `number` + `string` (or `string` + `number`) performs a string concatenation and returns a `string`
- `string` + `string` performs a string concatenation and returns a `string`
- `number` (or `string`) + `array` adds value to the front of array
- `array` + `number` (or `string`) adds value to the back of array
- `array` + `array` performs concatenation and returns an `array` (order of elements: first array elements followed by second array elements)

#### Operator `-`
- `number` - `number` returns a `number` (binary subtraction)
- \- `number` returns a `number` (unary subtraction)
- \- `string` casts `string` as a `number` (if not possible an exception is thrown) and performs unary subtraction, returns a `number`
- `string` - `number` casts `string` as a `number` (if not possible an exception is thrown) and performs binary subtraction, returns a `number`
- `number` - `string` casts `string` as a `number` (if not possible an exception is thrown) and performs binary subtraction, returns a `number`
- `string` - `string` casts the values as `number` (if not possible an exception is thrown) and performs binary subtraction, returns a `number`
- `array` - `number` removes the element at specified index, `number` must be an integer value (otherwise an exception is thrown)

#### Operator `*`
- `number` * `number` performs the multiplication, returns a `number`
- `string` * `number` casts `string` as a `number` (if not possible an exception is thrown) and performs the multiplication
- `number` * `string` casts `string` as a `number` (if not possible an exception is thrown) and performs the multiplication
- `string` * `string` casts the values as `number` (if not possible an exception is thrown) and performs the multiplication

#### Operator `**`
- `number` ** `number` performs the power operation (first to the power of second), returns a `number`
- `string` ** `number` casts `string` as a `number` (if not possible an exception is thrown) and performs the power operation
- `number` ** `string` casts `string` as a `number` (if not possible an exception is thrown) and performs the power operation
- `string` ** `string` casts the values as `number` (if not possible an exception is thrown) and performs the power operation

#### Operator `/`
- `number` / `number` performs the division of the two numbers (second `number` must be a not zero value, otherwise an exception is thrown), returns a `number`
- `string` / `number` casts `string` as a `number` (if not possible an exception is thrown) and performs the division, returns a `number`
- `number` / `string` casts `string` as a `number` (if not possible an exception is thrown) and performs the division, returns a `number`
- `string` / `string` casts the values as `number` (if not possible an exception is thrown) and performs the division, returns a `number`

#### Operator `//`
- `number` // `number` performs the integer division of the two numbers (second `number` must be a not zero value, otherwise an exception is thrown), returns an integer `number`
- `string` // `number` casts `string` as a `number` (if not possible an exception is thrown) and performs the integer division, returns a `number`
- `number` // `string` casts `string` as a `number` (if not possible an exception is thrown) and performs the integer division, returns a `number`
- `string` // `string` casts the values as `number` (if not possible an exception is thrown) and performs the integer division, returns a `number`

#### Operator `%`
- `number` % `number` performs the modulus division (double numbers included) of the two numbers (second `number` must be a not zero value, otherwise an exception is thrown), returns a `number`
- `string` % `number` casts `string` as a `number` (if not possible an exception is thrown) and performs the modulus division, returns a `number`
- `number` % `string` casts `string` as a `number` (if not possible an exception is thrown) and performs the modulus division, returns a `number`
- `string` % `string` casts the values as `number` (if not possible an exception is thrown) and performs the modulus division, returns a `number`

#### Operator `%%`
- `number` %% `number` performs the modulus division (double numbers included) of the two numbers (second `number` must be a not zero value, otherwise an exception is thrown), returns an integer `number`
- `string` %% `number` casts `string` as a `number` (if not possible an exception is thrown) and performs the modulus division, returns an integer `number`
- `number` %% `string` casts `string` as a `number` (if not possible an exception is thrown) and performs the modulus division, returns an integer `number`
- `string` %% `string` casts the values as `number` (if not possible an exception is thrown) and performs the modulus division, returns an integer `number`

### Logic operators
#### Operator `<`
- `number` < `number` performs the logic less operation, returns an integer `number` with value {0, 1}
- `string` < `number` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `number` < `string` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `string` < `string` performs the string character-to-character comparison, returns an integer `number` with value {0, 1}

#### Operator `<=`
- `number` <= `number` performs the logic less or equal operation, returns an integer `number` with value {0, 1}
- `string` <= `number` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `number` <= `string` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `string` <= `string` performs the string character-to-character comparison, returns an integer `number` with value {0, 1}

#### Operator `>`
- `number` > `number` performs the logic greater operation, returns an integer `number` with value {0, 1}
- `string` > `number` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `number` > `string` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `string` > `string` performs the string character-to-character comparison, returns an integer `number` with value {0, 1}

#### Operator `>=`
- `number` >= `number` performs the logic greater or equal operation, returns an integer `number` with value {0, 1}
- `string` >= `number` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `number` >= `string` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `string` >= `string` performs the string character-to-character comparison, returns an integer `number` with value {0, 1}

#### Operator `==`
- `number` == `number` performs the logic equal operation, returns an integer `number` with value {0, 1}
- `string` == `number` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `number` == `string` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `string` == `string` performs the string character-to-character comparison, returns an integer `number` with value {0, 1}

#### Operator `!=`
- `number` != `number` performs the logic not equal operation, returns an integer `number` with value {0, 1}
- `string` != `number` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `number` != `string` casts `number` as `string` and performs the string character-to-character comparison, returns an integer `number` with value {0, 1}
- `string` != `string` performs the string character-to-character comparison, returns an integer `number` with value {0, 1}

#### Operator `AND`
- `number` AND `number` performs the logic and operation, returns an integer `number` with value {0, 1} (every number has logical value `1` if `number != 0`)
- `string` AND `number` sequentially execute this steps
    1. Try to cast `string` as `number`, if success perform logical and operation
    2. Cast `number` as `string` and perform logical and operation
    3. Return an integer `number` with value {0, 1}
- `number` AND `string` sequentially execute this steps
    1. Try to cast `string` as `number`, if success perform logical and operation
    2. Cast `number` as `string` and perform logical and operation
    3. Return an integer `number` with value {0, 1}
- `string` AND `string` checks if both strings have length > 0, returns an integer `number` with value {0, 1}

#### Operator `OR`
- `number` OR `number` performs the logic and operation, returns an integer `number` with value {0, 1} (every number has logical value `1` if `number != 0`)
- `string` OR `number` sequentially execute this steps
    1. Try to cast `string` as `number`, if success perform logical and operation
    2. Cast `number` as `string` and perform logical and operation
    3. Return an integer `number` with value {0, 1}
- `number` OR `string` sequentially execute this steps
    1. Try to cast `string` as `number`, if success perform logical and operation
    2. Cast `number` as `string` and perform logical and operation
    3. Return an integer `number` with value {0, 1}
- `string` OR `string` checks if at least one of the strings have length > 0, returns an integer `number` with value {0, 1}

### Assignations (operator `<-`)
#### Variable assignation
- `lhs` <- `rhs` assigns `rhs` value to `lhs`. `lhs` must be a valid variable reference (variable to assign to or variable array index(es) for specific element assignation)

#### Variable declaration
- `<variable_name>` <- `<value>` declares a variable if not previously declared

#### Empty array declaration
- `<variable_name>[BEGIN]` <- `<value>` declares an empty array if not previously declared
- Note: `<value>` is discarded

#### Filled array declaration
- `<variable_name>[<index>]` <- `<value>` declares an array with `<index> + 1` length filled with `<value>` values if not previously declared. 
- Note: this operation is valid only during array declaration otherwise it's threated as a [variable assignation](#variable-assignation)

### Other operators
#### Operator `?` (length/size operator)
- `array` ? returns an integer `number` representing the length of the `<array>`
- `string` ? returns an integer `number` representing the length of the `string`


#### Operator `[]` (index operator)
- `array` [`<index_expression>`] <- `<expression>` assigns `<expression>` value to specific `array` location specified by `<index_expression>`
    - `<index_expression>` must return an integer value within `array` range [0, array_size], otherwise an exception is thrown
- `dest_string` [`<index_expression>`] <- `string` assigns `string` value to specific `dest_string` location specified by `<index_expression>`
    - `<index_expression>` must return an integer value within `dest_string` range [0, string_size], otherwise an exception is thrown
    - `string` must be a *single character string*, otherwise an exception is thrown

#### Operator `[]` (character delete operator)
- `string` [`<index_expression>`] <- `""` performs the deletion of the character at specific index specified by `<index_expression>`
    - `<index_expression>` must return an integer value within `dest_string` range [0, string_size], otherwise an exception is thrown

#### Operator `()` (function operator)
- `<function_name>([<arg1>, ...])` calls a function
    - If args present they're passed as copy values

## Flow control constructs
### While statement
A while loop is a loop in which the condition is tested at the start of the block. While condition is true
the instructions contained within the loop will be executed. After the `WHILE (<condition>)` statement you need to add the `BEGIN` and `END` keywords to define the while block in which the instructions are contained
```
# while loop
x <- 5
WHILE (x > 0)
BEGIN
    write x, "\n"
    x <- x - 1
END
```
output:
```
5
4
3
2
1

```

### Do-while statement
A do-while loop is a loop in which the condition is tested at the end of the block. Until the condition is false the instructions contained within the loop will be executed
```
# do-while loop
x <- 5
REPEAT
    exit <- 0
    write x, "\n"
    x <- x - 1
    IF (x == 2) THEN
        exit <- 1
    ENDIF
UNTIL (exit == 1)
```
output:
```
5
4
3

```

### If statement
An if statement is a control construct in which the instructions within each block are executed if and only if the tested condition for that block is true, and only the first true condition will be executed leaving the following ones with no execution. After an `IF (<condition>) THEN` you can add one and only one `ELSE` statement (not mandatory). After an `IF (<condition>) THEN` you can add one or multiple `ELIF (<condition>) THEN` (not mandatory). At the end of the statement you need to close it with the mandatory `ENDIF` keyword
```
# if with an elif and else
x <- 5
IF (x < 5) THEN
    write "x < 5"
ELIF (x > 5) THEN
    write "x > 5"
ELSE
    write "x == 5"
ENDIF
```
output: `x == 5`
```
# if with an elif
x <- 3
IF (x < 5) THEN
    write "x < 5"
ELIF (x == 3) THEN
    write "x == 3"
ENDIF
```
output: `x < 5`

## I/O
### Input
To read from the standard input (keyboard) the `read` instruction is used. Syntax is `read <var1>, <var2>, ...` and all the specified variables, if not previously declared, will be like it is a normal variable declaration
```
# Read one input
read input      # input variable is now declared
write input, "\n"
```
input: `2.523`

output: `2.523`
```
# Read multiple inputs
input1 <- 5
write input1, "\n"
read input1, input2
write "1) ", input1, "\n2) ", input2
```
input: `Hello world!`, `32`

output:
```
5
1) Hello world!
2) 32
```

### Output
To write to the standard output the `write` instruction is used. Syntax is `write "<string literal>", <expression>, ...`. All used variables must be declared before this instruction is executed, otherwise it will throw an error. To use escaped characters you need to use the `\` as escape character
```
# Write example
integer <- 5
double <- 2.52
string <- "string value"
array[2] <- 69

write "Hello world!"
write "\nI'm adding", "another", "row\n"
write integer, double
write "\nString: ", string, "\nArray: ", array, "\n"
write "Expression: ", 1 + 2, " <- should be \"3\""
```
output:
```
Hello world!
I'm addinganotherrow
52.52
String: string value
Array: [69, 69, 69]
Expression: 3 <- should be "3"
```

## Functions
### Declaration
Functions are declared with the `FUNCTION` keyword, following the syntax `FUNCTION <name>([<parameter1>, <parameter2>, ...])`. They must be followed by an instruction block defined by `BEGIN` and `END`. **Functions can only be declared in the global scope and can't be declared inside other functions**
```
# Empty parameter function with no return
FUNCTION no_params()
BEGIN
    write "no params!"
    RETURN
END

# One parameter function with no return
FUNCTION one_param(param)
BEGIN
    param <- param + 1
    write param
END

# More parameters function with expression return
FUNCTION more_params(p1, p2, p3)
BEGIN
    write "p1: ", p1, "\np2: ", p2, "\np3: ", p3, "\n"
    RETURN p1 + p2 + p3 + 4
END
```
### Other information
- Name: can start with `_` and capital/lower case letters. Following allowed characters are `_`, capital/lower letters and numbers
- Parameters: values are passed by copy. This language doesn't currently support reference types so if variables need to be changed outside of the function's scope it can be done by using global variables
- Return: a function can return a single value (can be an expression) by using `RETURN <expression>` and the execution can be stopped before reaching the last line by using `RETURN` keyword without any expression (this means that the function returns no value). If no `RETURN` is executed before the `END` of the block the function returns no value (like using `RETURN` before the `END` keyword)
- Function call:
    - Execution: just call the function `<function>(<params>)`
    - Expression: you can call functions in expressions (eg. `x <- 1 + <function>(<params>)`). If function doesn't return a value an exception is thrown

## Scopes
### Global scope
Global scope is the most external scope in which function declarations and global variables exist. Function and global variables position within the file is irrelevant for execution because the parser parses every line before the execution

Global variables are declared variables that are available inside every function, which can read and modify the global variable value to share the new value with other functions.

Global variables takes the priority on function variables so if a function has a variable declaration inside that has the same name as the global variable that is not a declaration but a normal assignation (global variable value is modified)
```
# Global variables and functions
global_1 <- "Hello!"

FUNCTION main()
BEGIN
    write global_1, " ", global_2
    global_2 <- global_1
    function()
END

global_2 <- 5

FUNCTION function()
BEGIN
    write "\n", global_2
END
```
output: 
```
Hello! 5
Hello!
```

### Function scope
Function scope is a scope in which expressions and flow control instructions live. Every function has its personal scope. Every function can declare its own variables for execution and every flow control block can access variables present within the function
```
FUNCTION main()
BEGIN
    # this is the main function scope
    read x
    IF (x < 5)
        # this is still the same function scope
    ELIF (x > 5)
        # this is also the same function scope
    ELSE
        # ...
    ENDIF
END
```

## Program
The program entry point is the **first** function block encountered by the parser. This special function can be declared in two ways:
- One and only one parameter: that parameter will contain an array with execution arguments provided via command line (treated as string values) without the interpreter executable and without the file name
- No parameters: if parameters provided they will be discarded by the interpreter
```
# Entry point with no arguments

FUNCTION main()
BEGIN
    write "Executed the main function!"
END

FUNCTION not_main()
BEGIN
    write "Executed the not_main function!"
END
```
command: `./interpreter file.txt`
output: `Executed the main function!`
```
# Entry point with arguments

FUNCTION main(args)
BEGIN
    write args
END

FUNCTION not_main()
BEGIN
    write "Executed the not_main function!"
END
```
command: `./interpreter file.txt arg1 2 3.25`
output: `['arg1', '2', '3.25']`