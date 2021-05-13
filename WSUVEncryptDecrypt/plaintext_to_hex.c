#include "mylib.h"


void plaintext_to_hex(char* plaintext_file, char* hex_file){
	FILE *plaintext_fp;
	FILE *hex_file_fp;

	plaintext_fp = fopen(plaintext_file,"r");
	hex_file_fp = fopen(hex_file,"w+");
	unsigned short int c;
	char hex_string[16];

	do {
		c = fgetc(plaintext_fp); // get a character from the plaintext
		sprintf(hex_string,"%02x",c);
		if ( feof(plaintext_fp) || (strcmp(hex_string,"ffff") == 0) ){
                        break;
                }	
		fputs(hex_string,hex_file_fp);		
	} while(1);

	fclose(plaintext_fp);
	fclose(hex_file_fp);

	return;
}
