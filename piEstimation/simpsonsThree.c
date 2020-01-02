#include "mylib.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288   /*pi*/
#endif
// Based on cubic interpolation. Function returns the absolute error of the estimate and M_PI.

long double simpsonsThree(long double(*f)(long double), long double a, long double b, int n){

        // n is the number of partititions
        long double h=(b-a)/n; // h is the width of each partition
        int i=1;
        long double answer=0; 
        long double x;
        long double inc=h;

        while ( i < n ){ // Sum up each partition, adjusting the weights by the iteration.
                if ( i%3 == 0 ){ // If the iteration is a multiple of 3, multiply by 2
                        x=a+inc;
                        answer=answer+(2.0L*f(x));
                }
                else if ( i%3 != 0 ){ // If not, multiply it by three.
                        x=a+inc;
                        answer=answer+(3.0L*f(x));
                }
                i++;
                inc += h;
        }
        answer=h*(3.0L/8.0L)*(answer+f(a)+f(b)); // Final estimate

        return fabsl(answer-M_PI);

}

