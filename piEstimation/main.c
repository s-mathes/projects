//Sarah Mathes
//CS330 - Numerical Computing, WSUV Fall 2019
//____________________________________________________
//|                                                  |
//|   To compile:                                    |
//|   -Use command 'make' to execute the Makefile    |
//|   -Makefile will make an executable 'main'       |
//|__________________________________________________|
//
// Main program drives the loop for n=1 to n=16 
// Calls each functions and prints the return values, the absolute error,
// in a row.
// 
#include "mylib.h"
#include <math.h>

int main(void){

	int i=0;
	int n=12*pow(2,i);
	long double trap=0;
	long double simpOne;
	long double simpThree;
	long double booles;

	while ( i <= 16 ){
		n=12*pow(2,i);
		trap=trapezoid(f,0L,1L,n);
		simpOne=simpsonsOne(f,0L,1L,n);
		simpThree=simpsonsThree(f,0L,1L,n);
		booles=Booles(f,0L,1L,n);
		printf("%d ", n);
	      	printf("%0.10Le  %0.10Le  %0.10Le  %0.10Le\n", trap, simpOne, simpThree, booles);	
		i++;
	}

	return(0);
}
