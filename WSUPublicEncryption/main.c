#include "mylib.h"

void main(int argc, char* argv[]){
	char *key, *input_file, *output_file;
        time_t t;	
	srand((unsigned) time(&t));
	int mode=0;

	for( int i=0 ; i<argc ; i++ ){
		if( strcmp(argv[i],"-genkey") == 0 ){
			printf("MODE: Generating key, please stand by...\n");
			mode=1;
		}
		if ( strcmp(argv[i],"-e") == 0 ){
			printf("MODE: Encrypting\n");
			mode=2;
		}
		if( strcmp(argv[i],"-d") == 0 ){
			printf("MODE: Decrypting\n");
			mode=3;
		}
		if ( strcmp(argv[i],"-k") == 0 ){
			key=argv[i+1];
			printf("Key: %s\n", key);
		}
		if ( strcmp(argv[i],"-in") == 0 ){
                        input_file=argv[i+1];
                        printf("Input File: %s\n", input_file);
                }
		if ( strcmp(argv[i],"-out") == 0 ){
                        output_file=argv[i+1];
                        printf("Output file: %s\n", output_file);
                }
	}
	if ( mode == 0 ){
		printf("Usage:\n");
		printf("./main -genkey\n");
		printf("./main -e -k pubkey.txt -in <input file> -out <output file>\n");
		printf("./main -d -k prikey.txt -in <input file> -out <output file>\n");
		return;
	}

	if ( mode == 1 ){ // Generating keys
		generate();
	}
       	if ( mode == 2 ){
		printf("Encrypting.\n");
		encrypt(input_file,output_file,key);
	}
	if ( mode == 3 ){
		printf("Decrypting.\n");
		decrypt(input_file,output_file,key);
	}

}
