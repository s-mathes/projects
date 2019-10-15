// Sarah Mathes
// CS 330, Fall 2019
// Programming Assignment 1
// Exercise 4.2
// 
// A C program that implement Horner's algorithm to process the Taylor
// Series function for y=e^x, expanded out to the n=6 term.
// The Taylor Series ( expanded to n=6) for y=e^x, rewritten as y=e^u, 
// where u=w*ln2, w=z-m, z=(x/ln2) and m=round(z), is as follows:
// f(u)=1+u+(1/2)u^2+(1/6)u^3+(1/24)u^4+(1/120)u^5+(1/720)u^6
//
// Test the program with the provided perl script, test.pl,
// to make sure that the result from myexp.c is within the
// smallest machine error of y=e^x.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_LOG2E
#define M_LOG2E   1.44269504088896340735992468100189214
#endif

#ifndef M_LN2
#define M_LN2  0.693147180559945309417232121458176568
#endif

float myexp(float x) {
        double coefficients[]={ 1, 1, (1.0/2.0), (1.0/6.0), (1.0/24.0), (1.0/120.0) , (1.0/720.0) }; 
	// The array of coefficients for u^0, u^1, u^2, u^3, u^4, u^5, u^6, respectively.
	double sum=0;
	int i=6; // 6 polynomials
	double z=(x/M_LN2);
	double m=round(z); // m will be the rounded whole number of z.
	double w=z-m;      // w will be the fractional part of z.
	double fx1= ldexp(1,m);  // 2^m, the first part of Equation 7.
	double u=w*M_LN2; // u=w*ln(2)
	
	while ( i >= 0 ){ // Horner's Algorithm
		sum=fmaf(sum,u,coefficients[i]); // ( sum * u ) + coeff[i];
		i--;
	}
	
	return(fx1*sum);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <number>\n", argv[0]);
		exit(1);
}
	
	const float x = atof(argv[1]);
 	const float y = myexp(x);

	printf("%0.9e\n", y);

 	return 0;
}

