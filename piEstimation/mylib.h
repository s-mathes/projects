#include <stdio.h>
#include <math.h>

long double Booles(long double(*f)(long double), long double a, long double b, int n);
double f2(double x);
long double simpsonsThree(long double(*f)(long double), long double a, long double b, int n);
long double f(long double x);
long double trapezoid(long double(*f)(long double), long double a,long double b, int n);
long double simpsonsOne(long double(*f)(long double), long double a,long double b, int n);
