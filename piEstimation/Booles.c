#include "mylib.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288   /*pi*/
#endif
// Function returns the absolute error between the estimate and M_PI.
// Boole's evaluates the function at 5 evenly spaced points in each partition.

long double Booles(long double(*f)(long double), long double a, long double b, int n){

        // n is the number of partititions
        long double h=(b-a)/n; // h is the width of each partition
        int i=1;
        long double answer=0;
        long double x;
        long double inc=h;

        while ( i < n ){ // sum up each partition, with different weight for each iteration.
                if ( i%4 == 0 ){ // If the iteration is a multiple of 4, multiply by 14
                        x=a+inc;
                        answer=answer+(14.0L*f(x));
                }
		else if ( i%2 != 0  ){ // If the iteration is odd, multiple by 32
			x=a+inc;
			answer=answer+(32.0L*f(x));
		}
                else { // If the iteration is anything else, multiply by 12
                        x=a+inc;
                        answer=answer+(12.0L*f(x));
                }
                i++;
                inc += h;
        }
        answer=h*(2.0L/45.0L)*(answer+7*f(a)+7*f(b)); // Final estimate

        return fabsl(answer-M_PI);

}

