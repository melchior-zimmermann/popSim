This software comes with absolutely no warranty.

For all questions and comments, please send an e-mail to popsimproject<at>gmail.com

The rk4 solver was inspired by the code found at http://people.sc.fsu.edu/~jburkardt/cpp_src/rk4/rk4.html , which is published under a LGPL license, and was written by J. Burkardt.

compiler: g++ (if you are trying to compile this on macOS, change the compiler to 'clang' in line 5 of the makefile)

Compiler flags: -std=c++14 -Wall -Wextra -g

Changes:
    - Adaption to current environment does not play a role in migration anymore (too many 'nan's)

Known issues:

    - Folders are not created recursively
    - somewhat fragile parsing
    
