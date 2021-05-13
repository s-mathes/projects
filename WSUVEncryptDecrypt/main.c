#include "mylib.h"

void main(int argc, char *argv[]){
	FILE* fp; FILE* output_fp;
	char ch;
	char temp[5]; // holds result of xor results
	strcpy(temp,"");
	char hex_key[17];
	char hex_key_0[5], hex_key_1[5], hex_key_2[5], hex_key_3[5];
	char plaintext[17];
	char plaintext_w0[5], plaintext_w1[5], plaintext_w2[5], plaintext_w3[5];
	int j=0; char mode;
	char* input_file;
	char* output_file;
	char* key_file;

	if ( argc != 8 ){
                printf("Usage:\n./wsu-crypt [ -e | -d ] -k [key.txt] -in [input.txt] -out [output.txt]\n");
		return;
	}

	for ( int i=0; i<argc ; i++){
		//printf("argc %i: %s\n", i, argv[i]);
		if ( strcmp(argv[i],"-e") == 0 ){
			mode = 'e';
			printf("Mode: ENCRYPT\n");
		}
		if ( strcmp(argv[i],"-d") == 0 ){
			mode = 'd';
			printf("Mode: DECRYPT\n");
		}
		if ( strcmp(argv[i],"-k") == 0 ) {
			key_file = argv[i+1];
			printf("Key file: %s\n", key_file);	
		}
		if ( strcmp(argv[i],"-in") == 0 ){
			input_file = argv[i+1];
			printf("Input file: %s\n",input_file);
		}
		if ( strcmp(argv[i],"-out") == 0 ){
			output_file = argv[i+1];
			printf("Output file: %s\n",output_file);
		}
	}

	if ( key_file == "" || input_file == "" || output_file == "" ){
		printf("Usage:\n./wsu-crypt [ -e | -d ] -k [key.txt] -in [input.txt] -out [output.txt]\n");
		return;
	}
	// Read the key from 'key.txt' into a string.
	fp = fopen(key_file, "r");
	fgets(hex_key,17*sizeof(char),fp);
	fclose(fp);
	
	// Split into w0, w1, w2, w3, w4	
	for (int i=0; i<4; i++,j++){ hex_key_0[i] = hex_key[j];}
        hex_key_0[4] = '\0';
	for (int i=0; i<4 ; i++,j++){ hex_key_1[i] = hex_key[j];}
	hex_key_1[4] = '\0';
        for (int i=0; i<4 ; i++,j++){ hex_key_2[i] = hex_key[j];}
        hex_key_2[4] = '\0';
        for (int i=0; i<4 ; i++,j++){ hex_key_3[i] = hex_key[j];}
        hex_key_3[4] = '\0';
	printf("KEY: %s %s %s %s \n", hex_key_0, hex_key_1, hex_key_2, hex_key_3);

	// Open up the outputfile.
	output_fp = fopen(output_file,"w+");
	if ( output_fp == NULL ) { printf("Couldn't open output file."); return; }

	// Read in plaintext
	fp = fopen(input_file,"r");
	if ( fp == NULL ) { printf("Couldn't open input file.\n"); return; }

	if ( mode == 'd' ){
		printf("DECRYPTING.\n");
		while ( 1 ){
                        if ( fgets(plaintext,17,fp) != NULL ){ // plaintext is actually ciphertext
				if (plaintext[0] == 10 ) break;
                                //printf("plaintext[0] = %i\n",plaintext[0]);
                                // Split plaintext into words
                                j=0;
                                for (int i=0; i<4; i++,j++) plaintext_w0[i] = plaintext[j];
                                plaintext_w0[4] = '\0';
                                for (int i=0; i<4; i++,j++) plaintext_w1[i] = plaintext[j];
                                plaintext_w1[4] = '\0';
                                for (int i=0; i<4; i++,j++) plaintext_w2[i] = plaintext[j];
                                plaintext_w2[4] = '\0';
                                for (int i=0; i<4; i++,j++) plaintext_w3[i] = plaintext[j];
                                plaintext_w3[4] = '\0';

                                int r0, r1, r2, r3;
                                int k=1;

                                r0 = hex4_to_dec(hex_key_0) ^ hex4_to_dec(plaintext_w0);
                                r1 = hex4_to_dec(hex_key_1) ^ hex4_to_dec(plaintext_w1);
                                r2 = hex4_to_dec(hex_key_2) ^ hex4_to_dec(plaintext_w2);
                                r3 = hex4_to_dec(hex_key_3) ^ hex4_to_dec(plaintext_w3);

				while ( k<17 ){
                                        // convert the key to decimal, idk why the last 3 need to be manual
                                        unsigned long long int big = hex16_to_dec(hex_key);

                                        int f0f1;
                                        f0f1 = f(r0,r1,big,17-k,'d');
                                        char f0f1_hex[9]; sprintf(f0f1_hex,"%08x",f0f1);
                                        char f0_hex[5]; char f1_hex[5];
                                        f0_hex[0]=f0f1_hex[0];  f1_hex[0]=f0f1_hex[4];
                                        f0_hex[1]=f0f1_hex[1];  f1_hex[1]=f0f1_hex[5];
                                        f0_hex[2]=f0f1_hex[2];  f1_hex[2]=f0f1_hex[6];
                                        f0_hex[3]=f0f1_hex[3];  f1_hex[3]=f0f1_hex[7];
                                        f0_hex[4]='\0';         f1_hex[4]='\0';
                                       // printf("f0 and f1 split up: %s %s\n", f0_hex,f1_hex);

					unsigned short int r3_xor_f1 = r3 ^ hex4_to_dec(f1_hex);
					unsigned short int shifted_r3_xor_f1 = (r3_xor_f1 >> 1) | (r3_xor_f1 << (16-1));

					unsigned short int r2_prime = (r2 << 1) | (r2 >> (16-1)); // rotate r2 left
					unsigned short int r2_prime_xor_f0 = r2_prime ^ hex4_to_dec(f0_hex); 

                                        r2=r0;
                                        r3=r1;
                                        r0=r2_prime_xor_f0;
                                        r1=shifted_r3_xor_f1;
                                        //printf("after round %i: %04x %04x %04x %04x\n\n",k-1, r0,r1,r2,r3);
                                        k++;
                                }
				 unsigned short int k0,k1,k2,k3;
                                unsigned short int y0,y1,y2,y3;
                                unsigned short int c0,c1,c2,c3;
                                char c0_str[5],c1_str[5],c2_str[5],c3_str[5];

                                y0=r2; y1=r3; y2=r0; y3=r1;
                                k0=hex4_to_dec(hex_key_0);
                                k1=hex4_to_dec(hex_key_1);
                                k2=hex4_to_dec(hex_key_2);
                                k3=hex4_to_dec(hex_key_3);
                                c0 = y0 ^ k0;
                                c1 = y1 ^ k1;
                                c2 = y2 ^ k2;
                                c3 = y3 ^ k3;
                                char output[17];
                                sprintf(c0_str,"%04x",c0);sprintf(c1_str,"%04x",c1);sprintf(c2_str,"%04x",c2);sprintf(c3_str,"%04x",c3);
                                strcpy(output,c0_str); strcat(output,c1_str); strcat(output,c2_str); strcat(output,c3_str);
				char one[3],two[3],three[3],four[3],five[3],six[3],seven[3],eight[3];
				one[0]=c0_str[0]; one[1]=c0_str[1]; one[3]='\0';
				two[0]=c0_str[2]; two[1]=c0_str[3]; two[3]='\0';
				three[0]=c1_str[0]; three[1]=c1_str[1]; three[3]='\0';
                                four[0]=c1_str[2]; four[1]=c1_str[3]; four[3]='\0';
				five[0]=c2_str[0]; five[1]=c2_str[1]; five[3]='\0';
                                six[0]=c2_str[2]; six[1]=c2_str[3]; six[3]='\0';
				seven[0]=c3_str[0]; seven[1]=c3_str[1]; seven[3]='\0';
                                eight[0]=c3_str[2]; eight[1]=c3_str[3]; eight[3]='\0';

				char *ptr;
				
	//			printf("attemp: %s %s %s %s %s %s %s %s\n",one,two,three,four,five,six,seven,eight);
				int ascii1,ascii2,ascii3,ascii4,ascii5,ascii6,ascii7,ascii8;
				ascii1=strtol(one,NULL,16);
                                ascii2=strtol(two,NULL,16);
                                ascii3=strtol(three,NULL,16);
                                ascii4=strtol(four,NULL,16);
                                ascii5=strtol(five,NULL,16);
                                ascii6=strtol(six,NULL,16);
                                ascii7=strtol(seven,NULL,16);
                                ascii8=strtol(eight,NULL,16);
	//			printf("did it work? %i %i %i %i %i %c %c %c\n",ascii1,ascii2,ascii3,ascii4,ascii5,ascii6,ascii7,ascii8);
			
				char final_string[9];
				final_string[0]=ascii1; 
				final_string[1]=ascii2;
				final_string[2]=ascii3;
				final_string[3]=ascii4;
				final_string[4]=ascii5;
				final_string[5]=ascii6;
				final_string[6]=ascii7;
				final_string[7]=ascii8;
                                final_string[8]='\0';
				printf("Plaintext: %s\n",final_string);
				fputs(final_string,output_fp);
				//fflush(output_fp);

			} else {  break; }
		}
	} else if ( mode == 'e' ){
		printf("ENCRYPTING.\n");
		plaintext_to_hex(input_file,"temp_hex_file.txt");
		fp = fopen("temp_hex_file.txt","r");

		int adj1=0;
		while ( 1 ){
			if ( fgets(plaintext,17,fp) != NULL ){ 
				if (plaintext[0] == 10 ) break;
				if ( strlen(plaintext) < 16 ){
					int str = strlen(plaintext)-1;
					for ( int i=(strlen(plaintext)) ; i<16 ; i++ ){
						if ( i == 7 ){
							adj1 = 8-str;
                                                        char num = '0'+adj1;
							plaintext[i]=num;
						}
						else if ( i == 15 ){
							int adj = 16-str-adj1;
							char num = '0'+adj;
							plaintext[i]=num;
						}
						else plaintext[i]='0';
					}
					plaintext[16] = '\0';
				}

				// Split plaintext into words
				j=0;
				for (int i=0; i<4; i++,j++) plaintext_w0[i] = plaintext[j];
				plaintext_w0[4] = '\0';
				for (int i=0; i<4; i++,j++) plaintext_w1[i] = plaintext[j];
				plaintext_w1[4] = '\0';
				for (int i=0; i<4; i++,j++) plaintext_w2[i] = plaintext[j];
				plaintext_w2[4] = '\0';
				for (int i=0; i<4; i++,j++) plaintext_w3[i] = plaintext[j];
				plaintext_w3[4] = '\0';
				//printf("PLAINTEXT: %s %s %s %s\n", plaintext_w0, plaintext_w1, plaintext_w2, plaintext_w3);

				//printf("%i %i %i %i\n", hex4_to_dec(hex_key_0),hex4_to_dec(hex_key_1),hex4_to_dec(hex_key_2),hex4_to_dec(hex_key_3));
				
				int r0, r1, r2, r3;
				int k=1;


				r0 = hex4_to_dec(hex_key_0) ^ hex4_to_dec(plaintext_w0);
				r1 = hex4_to_dec(hex_key_1) ^ hex4_to_dec(plaintext_w1);
				r2 = hex4_to_dec(hex_key_2) ^ hex4_to_dec(plaintext_w2);
				r3 = hex4_to_dec(hex_key_3) ^ hex4_to_dec(plaintext_w3);

				//printf("After whiting:\nr0,r1,r2,r3: %0x %0x %0x %0x\n", r0,r1,r2,r3);
				while ( k<17 ){	
					// convert the key to decimal, idk why the last 3 need to be manual
					unsigned long long int big = hex16_to_dec(hex_key);

					int f0f1;
					f0f1 = f(r0,r1,big,k,'e');
					char f0f1_hex[9]; sprintf(f0f1_hex,"%08x",f0f1);
					char f0_hex[5]; char f1_hex[5];
					f0_hex[0]=f0f1_hex[0];	f1_hex[0]=f0f1_hex[4];
					f0_hex[1]=f0f1_hex[1];	f1_hex[1]=f0f1_hex[5];
					f0_hex[2]=f0f1_hex[2];	f1_hex[2]=f0f1_hex[6];
					f0_hex[3]=f0f1_hex[3];	f1_hex[3]=f0f1_hex[7];
					f0_hex[4]='\0';		f1_hex[4]='\0';
				//	printf("f0 and f1 split up: %s %s\n", f0_hex,f1_hex);

					unsigned short int shifted_r3 = (r3 << 1) | (r3 >> (16-1));  // rotate r3 once to the left.
					int r3_prime = shifted_r3 ^ hex4_to_dec(f1_hex); // xor r3 with f1

					unsigned short int r2_prime = r2 ^ hex4_to_dec(f0_hex); // xor f0 with r2
					r2_prime = (r2_prime >> 1) | (r2_prime << (16-1)); // rotate it one to the right

					r2=r0;	
					r3=r1;
					r0=r2_prime;
					r1=r3_prime;
					//printf("after round %i: %04x %04x %04x %04x\n\n",k-1, r0,r1,r2,r3);
					k++;
				}
				
				unsigned short int k0,k1,k2,k3;
				unsigned short int y0,y1,y2,y3;
				unsigned short int c0,c1,c2,c3;
				char c0_str[5],c1_str[5],c2_str[5],c3_str[5];

				y0=r2; y1=r3; y2=r0; y3=r1;
				k0=hex4_to_dec(hex_key_0);
				k1=hex4_to_dec(hex_key_1);
				k2=hex4_to_dec(hex_key_2);
				k3=hex4_to_dec(hex_key_3);
				c0 = y0 ^ k0;
				c1 = y1 ^ k1;
				c2 = y2 ^ k2;
				c3 = y3 ^ k3;
				char output[17];
				sprintf(c0_str,"%04x",c0);sprintf(c1_str,"%04x",c1);sprintf(c2_str,"%04x",c2);sprintf(c3_str,"%04x",c3);
				strcpy(output,c0_str); strcat(output,c1_str); strcat(output,c2_str); strcat(output,c3_str);
				printf("Ciphertext: %s\n", output);
				//printf("Printing to %s...\n",output_file);
				fputs(output, output_fp);
				plaintext[0]='\0';
			} else { 
				remove("temp_hex_file.txt");
				break; }
		}
	}

//	fclose(fp);
//	fclose(output_fp);
	return;
}
