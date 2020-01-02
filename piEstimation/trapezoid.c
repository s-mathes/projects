#include "mylib.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288   /*pi*/
#endif
//
// Divides the area under the curve into trapezoids, and calculates the area of each trapezoid.
// Function returns the absolute error of the estimate, and M_PI
long double trapezoid(long double(*f)(long double), long double a, long double b, int n){
	
	// n is the number of partititions
	const double h=(b-a)/n; // h is the width of each partition	
	int i=1; //i=0;
	long double answer=.5L*(f(a)+f(b));
	double x;
	long double inc=h;

	while ( i < n ){ // sum up each partition
		x = a + inc;
		answer=answer+f(x);
		i++;
		inc += h; 
	}
	answer=h*answer; // final estimate

	return fabsl(answer-M_PI);
	
}
