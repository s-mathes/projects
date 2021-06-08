#include "mylib.h"

// Used an algorithm based on Theorem 3 from essay 'Fast Primality
// Testing for Integers That Fit into a Machine Word':
// Theorem 3 (Jaeschke, Sinclair) : If n < 2^32 is a b-SPRP for b as an element
// of {2,7,61}, then n is a prime
//That is to say, that for n <  4,759,123,141, it is enough to witness-test 2, 7, and 61.
// 
// Jancina, Jakub, and Forisek, Michal. "Fast Primality Testing for Integers That Fit intoa Machine Word".
// 	Comenius University. http://ceur-ws.org/Vol-1326/020-Forisek.pdf
// Wikipedia. "Miller–Rabin primality test". 4/16/2021. https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
//
unsigned long long int miller_rabin(unsigned long long int n, int s){

	if ( n <= 1 ) return 0;
	if ( n == 2 ) return 1;
	
	if ( (witness(2,n) == false)  &&
	     (n <= 7 || witness(7,n) == false) &&
	     (n <= 61 || witness(61,n) == false ) ){
                //printf("%lld is probably prime.\n",n);
		return 0;
	     } else {
                //printf("%lld is composite.\n",n);
		return 1;
	     }

}
