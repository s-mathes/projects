#include "mylib.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288   /*pi*/
#endif
//
// Simpson's uses a quadratic interpolating polynomial to estimate the area of smaller
// partitions of the area under the curve, f(x). Return the absolute error of the estimate
// and the value of M_PI.

long double simpsonsOne(long double(*f)(long double), long double a, long double b, int n){

        // n is the number of partititions
        long double h=(b-a)/n; // h is the width of each partition
        int i=1;
        long double answer=0;
        long double x;
        long double inc=h;

        while ( i < n ){ // sum up each partition, adjust the weights based on the iteration.
		if ( i%2 == 0 ){ // If the iteration is even, multiply by 2
			x=a+inc;
			answer=answer+(2.0L*f(x));
		}
		else if ( i%2 != 0 ){ // If the iteration is odd, multiply by 4.
			x=a+inc;
			answer=answer+(4.0L*f(x));
		}
                i++;
                inc += h;
        }
        answer=(answer+f(a)+f(b))*(h/3.0L); // final estimate

        return fabsl(answer-M_PI); 

}

