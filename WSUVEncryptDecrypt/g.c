#include "mylib.h"

unsigned int g(unsigned int r0, int k, int k0, int k1, int k2, int k3, int round){	
	// FTable[rows][columns]
	char* ftable[16][17] = {
        	{"a3","d7","09","83","f8","48","f6","f4","b3","21","15","78","99","b1","af","f9"},
	        {"e7","2d","4d","8a","ce","4c","ca","2e","52","95","d9","1e","4e","38","44","28"},
        	{"0a","df","02","a0","17","f1","60","68","12","b7","7a","c3","e9","fa","3d","53"},
	        {"96","84","6b","ba","f2","63","9a","19","7c","ae","e5","f5","f7","16","6a","a2"},
        	{"39","b6","7b","0f","c1","93","81","1b","ee","b4","1a","ea","d0","91","2f","b8"},
	        {"55","b9","da","85","3f","41","bf","e0","5a","58","80","5f","66","0b","d8","90"},
        	{"35","d5","c0","a7","33","06","65","69","45","00","94","56","6d","98","9b","76"},
	        {"97","fc","b2","c2","b0","fe","db","20","e1","eb","d6","e4","dd","47","4a","1d"},
        	{"42","ed","9e","6e","49","3c","cd","43","27","d2","07","d4","de","c7","67","18"},
	        {"89","cb","30","1f","8d","c6","8f","aa","c8","74","dc","c9","5d","5c","31","a4"},
        	{"70","88","61","2c","9f","0d","2b","87","50","82","54","64","26","7d","03","40"},
        	{"34","4b","1c","73","d1","c4","fd","3b","cc","fb","7f","ab","e6","3e","5b","a5"},
	        {"ad","04","23","9c","14","51","22","f0","29","79","71","7e","ff","8c","0e","e2"},
        	{"0c","ef","bc","72","75","6f","37","a1","ec","d3","8e","62","8b","86","10","e8"},
       		{"08","77","11","be","92","4f","24","c5","32","36","9d","cf","f3","a6","bb","ac"},
        	{"5e","6c","a9","13","57","25","b5","e3","bd","a8","3a","01","05","59","2a","46"}
	};

//	printf("Entering g()...\n");
	char g1[3];
	char g2[3];
	//r0 is 'aaee', 16 bits
	char r0_dec[5];
	sprintf(r0_dec,"%04x",r0);
//	printf("r0: %s k0: %0x k1: %0x k2: %0x k3: %0x\n",r0_dec, k0,k1,k2,k3);

	g1[0]=r0_dec[0];	g1[1]=r0_dec[1];	g1[2]='\0';
	g2[0]=r0_dec[2];	g2[1]=r0_dec[3];	g2[2]='\0';

//	printf("%s %s -> %i %i\n", g1, g2, hex2_to_dec(g1), hex2_to_dec(g2));
//	printf("G1: %0x\n",(unsigned int)hex2_to_dec(g1));
//	printf("G2: %0x\n",(unsigned int)hex2_to_dec(g2));

	char g2_xor_k0_hex[3];
	int g2_xor_k0 = hex2_to_dec(g2) ^ k0;
//	printf("%0x xored with %0x: %02x\n", hex2_to_dec(g2),k0, g2_xor_k0);
	sprintf(g2_xor_k0_hex, "%02x", g2_xor_k0);
	//char temp_hex[3];
 	/*if ( strlen(g2_xor_k0_hex) == 1 ){ // needs a zero.
		strcpy(temp_hex,g2_xor_k0_hex);
		g2_
	}	*/
       
	char ftable_res[3];
        strcpy(ftable_res,ftable[hex_to_dec(g2_xor_k0_hex[0])][hex_to_dec(g2_xor_k0_hex[1])]); 
//	printf("FTABLE[%i][%i] = %s\n",hex_to_dec(g2_xor_k0_hex[0]),hex_to_dec(g2_xor_k0_hex[1]),ftable_res);
	unsigned int g3 = hex2_to_dec(ftable_res) ^ hex2_to_dec(g1);
	//printf("G3: %0x\n", g3);
        
	
	char g3_xor_k1_hex[3];
        unsigned int g3_xor_k1 = g3 ^ k1;
        //printf("\n%0x xored with %0x: %0x\n", g3,k1, g3_xor_k1);
        sprintf(g3_xor_k1_hex, "%02x", g3_xor_k1);
        strcpy(ftable_res,ftable[hex_to_dec(g3_xor_k1_hex[0])][hex_to_dec(g3_xor_k1_hex[1])]);
        //printf("FTABLE[%i][%i] = %s\n",hex_to_dec(g3_xor_k1_hex[0]),hex_to_dec(g3_xor_k1_hex[1]),ftable_res);
        unsigned int g4 = hex2_to_dec(ftable_res) ^ hex2_to_dec(g2);
        //printf("G4: %0x\n", g4);

	char g4_xor_k2_hex[3];
        unsigned int g4_xor_k2 = g4 ^ k2;
        //printf("\n%0x xored with %0x: %0x\n", g4,k2, g4_xor_k2);
        sprintf(g4_xor_k2_hex, "%02x", g4_xor_k2);
        strcpy(ftable_res,ftable[hex_to_dec(g4_xor_k2_hex[0])][hex_to_dec(g4_xor_k2_hex[1])]);
        //printf("FTABLE[%i][%i] = %s\n",hex_to_dec(g4_xor_k2_hex[0]),hex_to_dec(g4_xor_k2_hex[1]),ftable_res);
        unsigned int g5 = hex2_to_dec(ftable_res) ^ g3; //hex2_to_dec(g3);
        //printf("G5: %0x\n", g5);
	char g5_hex[3]; sprintf(g5_hex,"%02x",g5);

	char g5_xor_k3_hex[3];
        unsigned int g5_xor_k3 = g5 ^ k3;
        //printf("\n%0x xored with %0x: %0x\n", g5,k3, g5_xor_k3);
        sprintf(g5_xor_k3_hex, "%02x", g5_xor_k3);
        strcpy(ftable_res,ftable[hex_to_dec(g5_xor_k3_hex[0])][hex_to_dec(g5_xor_k3_hex[1])]);
        //printf("FTABLE[%i][%i] = %s\n",hex_to_dec(g5_xor_k3_hex[0]),hex_to_dec(g5_xor_k3_hex[1]),ftable_res);
        unsigned int g6 = hex2_to_dec(ftable_res) ^ g4; //hex2_to_dec(g3);
        //printf("G6: %0x\n", g6);
	char g6_hex[3]; sprintf(g6_hex,"%02x",g6);


	char t[9];
	strcpy(t,g5_hex); strcat(t,g6_hex);
//	printf("Final T: %s\n",t);
	

	return(hex4_to_dec(t));
}

