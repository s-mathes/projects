#include "mylib.h"


int hex4_to_dec(char* hex){

	int dec = 0;
	int power = 3;

	// a___ -> a*16^3
	for ( int i=0 ; i<4 ; i++){
		//printf("%c -> %i, to the power of %i\n", hex[i],hex_to_dec(hex[i]), power);
		dec = dec + hex_to_dec(hex[i])*pow(16,power);
		power--;
	}
	return dec;


}
