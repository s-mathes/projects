// Sarah Mathes
// CS 330, Fall 2019
// Programming Assignment 1
// Exercise 4.1
// A C program that implements the standard deviation equation for y=e^x
// Determine the smallest value for n that the relative error is guaranteed to be 
// below machine epsilon, .000000119209
//
// RESULTS OF RERR.C:
//----------------------------------------------------
// n= 1 Standard error= 0.060057
// n= 2 Standard error= 0.006938
// n= 3 Standard error= 0.000601
// n= 4 Standard error= 0.000042
// n= 5 Standard error= 0.000002
// n= 6 Standard error= 0.000000
// n= 7 Standard error= 0.000000
// n= 8 Standard error= 0.000000
// n= 9 Standard error= 0.000000
// n= 10 Standard error= 0.000000
// n= 11 Standard error= 0.000000
// n= 12 Standard error= 0.000000
// n= 13 Standard error= 0.000000
// n= 14 Standard error= 0.000000
// n= 15 Standard error= 0.000000
//----------------------------------------------------
//
// The smallest n such that the standard error is below machine epsilon
// is n=6.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double fact(int n) {
  if (n <= 1) return 1;
  return n*fact(n-1);
}

// Wrote my own pow() function, my bad.
// First argument is the base, second argument is the exponent.
double exponent(double number, int exponent){
	double sum=number;
	while ( exponent > 1 ){
		sum=sum*number;
		exponent--;
	}
	return(sum);
}

int main() {

	int n=1;
	double rel_err; 
	
	printf("----------------------------------------------------\n");
	while ( n <= 15 ){  // Calculate error for n equal to 1 through 15.
		rel_err=exponent((M_LN2/2),n+1)*(1/fact(n+1)); // Equation 12 from assignment description.
		printf("n= %d Standard error= %f\n", n, rel_err);
		n++;
	}	
        printf("----------------------------------------------------\n");

	return 0;
}

