#include "mylib.h"

unsigned long long int getq(void){
	char public_key_out[12] = "pubkey.txt"; // p g e2
        char private_key_out[12] = "prikey.txt"; // p g d
        unsigned long long int prime;
        bool isPrime = false;
        unsigned long long int p, q;
        time_t t;
        srand((unsigned) time(&t));

	// Find  a safe 32-bit prime
                while ( 1 ){ // Select a random 31-bit (k=32) prime
                        while ( 1 ){ // Find a prime number
                                q = (unsigned long long int)rand() | (0x1<<30);
                                //g = q;
				while ( q%3==0 || q%2==0 || q%5==0 || q%7==0 || q%11==0 || q%13==0 ){
                                        q = (unsigned long long int)rand() | (0x1<<30);
                                	//g = q;
				}
                                if ( miller_rabin(q,10000000) == 0 ) break;
                        }
                        if ( exponentiation(q,1,12) == 5 ){ // find qmod12=5
                                break;
                        }
                }

	return(q);



}
