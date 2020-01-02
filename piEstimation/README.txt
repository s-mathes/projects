Sarah Mathes
CS330 - Numerical Computing, WSUV Fall 2019
Program 3 - Pi via Integration
____________________________________________________
|                                                  |
|   To compile:                                    |
|   -Use command 'make' to execute the Makefile    |
|   -Makefile will make an executable 'main'       |
|__________________________________________________|

Implement four numerical methods for approximating definite integrals and
use them to estimate the value of pi. The four numerical methods are:

Trapezoid Composite
Simpson's 1/3 Composite
Simpson's 3/8 Composite
Boole's Composite

The methods take advantage of the following equivalence:
	pi = 4 * arctan(1) = integral( 4/(1+x^2) ) from 0 < x < 1
They estimate the integral by making increasingly smaller divisions in 
multiples of 12, up to 786432 divisions. Each method function returns the 
absolute error of the estimate and the actual value of pi. The main 
program prints each absolute error from each method in the following
format, for n=1 to n=16:

n  Trapezoid  Simpsons1/3  Simpsons3/8  Booles

