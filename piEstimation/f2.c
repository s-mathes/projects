#include "mylib.h"

double f2(double x){
	long double function=((8*(3*pow(x,2)-1))/pow(pow(x,2)+1,3));
	return function;
	//return -sin(x);
	//return -pow(M_E, x);
	//return (4*x*x - 2)*pow(M_E, -(x*x));
}
