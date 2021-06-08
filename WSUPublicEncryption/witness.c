#include "mylib.h"

bool witness(unsigned long long int d, unsigned long long int n){
	unsigned long long int d_pow, k, q;
	
	for(int i = 31; i >= 0; i--){
	    unsigned int pow2 = 0x1 << i; 
	    k = i;
	    if( pow2 > n ) continue;
	    q = (n-1)/pow2;
	    if( (q)*pow2 == (n-1) ) break;
	}

	d_pow = exponentiation(d, q, n);
	if ( d_pow == 1 ) return false;
	if (k) {
		for ( int i = 0 ; i < k-1 ; i++){
			if ( d_pow == n-1 ) return false;
			d_pow = exponentiation(d_pow, 2, n);
		}
	}
	if ( d_pow == n-1 ) return false;
	return true;
	/*unsigned long long int d_pow, k, q;
	unsigned long long int a=2+rand()%(n-4);
	unsigned long long int x = exponentiation(a,d,n);
	if ( x == 1 || x == (n-1) ) return true;
	printf("Entering witness...\n");

	while ( d != (n-1) ){		
		x = (x*x) % n;
		d = d*2;

		if ( x == 1 ) return false;
		if ( x == n-1 ) return true;
	}

	printf("Leaving witness...\n");
	return false;
*/
}
