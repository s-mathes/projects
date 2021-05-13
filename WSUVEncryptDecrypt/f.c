#include "mylib.h"

int f(int r0, int r1, unsigned long long key, int round,char mode){

	int g1keys[4]; int g2keys[4]; int fkeys[4];
	unsigned long long int shifted_key;
	shifted_key = key;

//	printf("ROUND: %i, ",round-1);
	if ( mode == 'd' ){	
		for ( int j=0 ; j<(round-1) ; j++){
                        for ( int i=0 ; i<12 ; i++ ){ // shift the key left 12 times for each round
                                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                        }
                }

		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g1keys[0] = k(shifted_key,4*round+3); // k0
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g1keys[1] = k(shifted_key,4*round+2); // k1
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g1keys[2] = k(shifted_key,4*round+1); // k2
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g1keys[3] = k(shifted_key,4*round+0); // k3
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));

                g2keys[0] = k(shifted_key,4*round+3); // k4
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g2keys[1] = k(shifted_key,4*round+2);
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g2keys[2] = k(shifted_key,4*round+1);
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                g2keys[3] = k(shifted_key,4*round+0);
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));

                fkeys[0] = k(shifted_key,4*round+3);
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                fkeys[1] = k(shifted_key,4*round+2);
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                fkeys[2] = k(shifted_key,4*round+1);
                shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
                fkeys[3] = k(shifted_key,4*round+0);


	} else if ( mode == 'e' ){
//		printf("*******encrypting in f()********\n");
		for ( int j=0 ; j<(round-1) ; j++){
			for ( int i=0 ; i<12 ; i++ ){ // shift the key left 12 times for each round
				shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));	
			}
		}

		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g1keys[0] = k(shifted_key,4*round+3); // k0
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g1keys[1] = k(shifted_key,4*round+2); // k1
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g1keys[2] = k(shifted_key,4*round+1); // k2
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g1keys[3] = k(shifted_key,4*round+0); // k3
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		
		g2keys[0] = k(shifted_key,4*round+3); // k4
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g2keys[1] = k(shifted_key,4*round+2);
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g2keys[2] = k(shifted_key,4*round+1);
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		g2keys[3] = k(shifted_key,4*round+0);
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		
		fkeys[0] = k(shifted_key,4*round+3); 
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		fkeys[1] = k(shifted_key,4*round+2); 
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		fkeys[2] = k(shifted_key,4*round+1); 
		shifted_key = (shifted_key << 1) | (shifted_key >> (64 - 1));
		fkeys[3] = k(shifted_key,4*round+0); 
	}

//	printf("Final: %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x %0x\n", g1keys[0], g1keys[1], g1keys[2], g1keys[3], g2keys[0],g2keys[1],g2keys[2],g2keys[3], fkeys[0],fkeys[1],fkeys[2],fkeys[3]);
	
	unsigned int t0 = g(r0,1,g1keys[0],g1keys[1],g1keys[2],g1keys[3],round);
	unsigned int t1 = g(r1,1,g2keys[0],g2keys[1],g2keys[2],g2keys[3],round);
	unsigned long long int two_tothe_sixteen = pow(2,16);	

//	printf("t0: %0x t1: %0x\n",t0,t1);
	char fkeys0[3],fkeys1[3]; 
	sprintf(fkeys0,"%02x",fkeys[0]); sprintf(fkeys1,"%02x",fkeys[1]);
	char concatted[5]; 
	strcpy(concatted,fkeys0); strcat(concatted,fkeys1);
	
//	printf("%0x + 2*%0x + (%s) mod 2^16\n", t0,t1,concatted);
	//printf("concatted keys: %s\n", concatted);
	long int f0 = (t0 + 2*t1 + hex4_to_dec(concatted)) % 65536;
//	printf("f0:  %0lx ",f0);

	char fkeys2[3],fkeys3[3];
        sprintf(fkeys2,"%02x",fkeys[2]); sprintf(fkeys3,"%02x",fkeys[3]);
        strcpy(concatted,fkeys2); strcat(concatted,fkeys3);
        
	
	// printf("concatted keys: %s\n", concatted);
        long int f1 = (2*t0 + t1 + hex4_to_dec(concatted)) % 65536;
//        printf("f1:  %0lx\n",f1);

//	printf("2*%0x + %0x + (%s) mod 2^16\n", t0,t1,concatted);

	char f0_hex[5]; char f1_hex[5];
	sprintf(f0_hex,"%04lx",f0); sprintf(f1_hex,"%04lx",f1);

	char result[9];
	strcpy(result,f0_hex); strcat(result,f1_hex);
//	printf("Sending out f0 and f1: %s\n", result);
	return(hex8_to_dec(result));

}
