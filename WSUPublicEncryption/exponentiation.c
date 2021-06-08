#include "mylib.h"

unsigned long long int exponentiation(unsigned long long int a, unsigned long long int b, unsigned long long int n){

        unsigned long long int c=0;
        unsigned long long int d=1;
	char temp[64];
	unsigned long long int mod; 
        int i=0;
        // convert b to a binary string
        while ( b > 0 ){
		mod = b%2;
		if ( mod == 1 ){
                	temp[i] = '1';
		}
		else if ( mod == 0 ){
			temp[i] = '0';
		}
                b=b/2;
                i++;
        }
	for ( int j=0; j<=strlen(temp); j++){
		if ( temp[i] != '0' || temp[i] != '1' ){
			temp[i]='\0';
		}
	}
	
	i=strlen(temp)-1;
	while ( i >= 0 ){ // go through each element of binary number
		c = 2*c;
		d = (d*d) % n;
		if ( temp[i] == '1' ){
			c = c+1;
			d=(d*a)%n;
		}
		i--;
	}
	return(d);

}
