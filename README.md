# postfixc
A reverse polish notation calculator written in C for use on the terminal and in scripts.

## Implemented operations:
    +, -, *, /, exp, pow, log, sqrt, inv,
    sin, cos, tan, asin, acos, atan, deg, rad
    
The purpose and arity of each operation should be self evident. All trig functions expect and return in radians.

## Built in constants:
    pi, e

## Compile with:
    $ gcc -o postfixc postfix.c -lm && strip postfixc

## Example usage:
    $ postfixc 15 7 1 1 + - / 3 \* 2 1 1 + + -
    5.0000
    
    $ postfixc 42 rad sin acos pi + deg
    228.000000
    
    $ e pi pow pi e pow
    23.140693 22.459158
    
    $ 0.5 inv
    2.000000
    
(Notice `\*` was used to prevent file globbing)