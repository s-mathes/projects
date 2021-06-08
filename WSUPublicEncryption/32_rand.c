#include "mylib.h"

unsigned long long int rand32(){
	unsigned long long int a;
	srand(time(NULL));

	a = (unsigned long long int)rand() | (0x1<<30);
	return a;

}
