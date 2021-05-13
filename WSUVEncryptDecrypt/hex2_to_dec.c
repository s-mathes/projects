#include "mylib.h"

// takes a two byte hex number, returns and in ex) ab - 171
int hex2_to_dec(char* hex){
	
	int dec = 0;
	int power = 1;

	for ( int i=0 ; i<2 ; i++){
		dec = dec + hex_to_dec(hex[i])*pow(16,power);
                power--;
	}
	return dec;
}
