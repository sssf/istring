IString
=======

A "better" string implementation in C. IStrings are compatable with the standard c string functions, 
but by using the standard c string functions might break the istring length invariant. After using 
the standard c string function use the istrfixlen function to restore the length invariant.


## Usage ##

Copy the source files into your project, include the "istring.h" 
and add the "istring.c" file when compiling your program.

Use the istring_mk function to create a new istring. You are then responsible to deallocate them using the istring_rm function.

For a complete list of istring functions see the documentation.


