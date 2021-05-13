#include "mylib.h"

unsigned long long int hex16_to_dec(char* hex){

        unsigned long long int dec = 0;
        int power = 15;
	unsigned long long int big=0;

        // a___ -> a*16^3
        for ( int i=0 ; i<13 ; i++){
               // printf("%c -> %i, to the power of %i\n", hex[i],hex_to_dec(hex[i]), power);
                dec = dec + hex_to_dec(hex[i])*pow(16,power);
                //printf("%llu\n",dec);
		power--;
        }
	unsigned long long int small = hex_to_dec(hex[13])*pow(16,2);
        unsigned long long int smaller = hex_to_dec(hex[14])*pow(16,1);
        unsigned long long int smallest = hex_to_dec(hex[15])*pow(16,0);
        dec = dec + small + smaller + smallest;

	return dec; //dec;

}
