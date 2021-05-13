#include "mylib.h"


unsigned int hex8_to_dec(char* hex){

	unsigned int dec = 0;
	int power = 7;

	// a___ -> a*16^3
	for ( int i=0 ; i<8 ; i++){
		//printf("%c -> %i, to the power of %i\n", hex[i],hex_to_dec(hex[i]), power);
		dec = dec + hex_to_dec(hex[i])*pow(16,power);
		power--;
	}
	return dec;


}
