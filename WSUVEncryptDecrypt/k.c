#include "mylib.h"
// key is the original, 16 character hex string
// subkeys is 192 elements

int k(unsigned long long int key, int x){ // x = 4*round
	char temp_hex[64];
	char hex_key[64];
	char temp[8][3];
	unsigned long long int shifted_left = key;
	int bit=x%8;

//	printf("BIT: %i\n",bit);
//	shifted_left = (key << 1) | (key >> (64 - 1));
//	printf("shifted key: %llu\n", shifted_left);
	sprintf(hex_key,"%016llx",shifted_left);
//	printf("k() %s length: %li\n",hex_key,strlen(hex_key));

	temp[0][0] = hex_key[0];
	temp[0][1] = hex_key[1];
	temp[0][2] = '\0';

	temp[1][0] = hex_key[2];
        temp[1][1] = hex_key[3];
        temp[1][2] = '\0';

	temp[2][0] = hex_key[4];
        temp[2][1] = hex_key[5];
        temp[2][2] = '\0';

	temp[3][0] = hex_key[6];
        temp[3][1] = hex_key[7];
        temp[3][2] = '\0';

	temp[4][0] = hex_key[8];
        temp[4][1] = hex_key[9];
        temp[4][2] = '\0';

	temp[5][0] = hex_key[10];
        temp[5][1] = hex_key[11];
        temp[5][2] = '\0';

	temp[6][0] = hex_key[12];
        temp[6][1] = hex_key[13];
        temp[6][2] = '\0';

	temp[7][0] = hex_key[14];
        temp[7][1] = hex_key[15];
        temp[7][2] = '\0';

	//return(hex2_to_dec(temp[bit]);	
//	printf("final: %s %i\n", temp[bit], hex2_to_dec(temp[bit]));

//	printf("BIT: %i, %s\n", bit, temp[bit]);
        return(hex2_to_dec(temp[bit]));



}
