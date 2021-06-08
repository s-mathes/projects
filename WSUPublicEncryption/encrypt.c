#include "mylib.h"

void encrypt(char *inputFile, char *outputFile, char *key){
	FILE *fp, *outfp;
	char *buffer, *token;
	size_t buffersize = 100;
	size_t chars;
	unsigned long long int privKeyHolder[3];
	unsigned long long int pubKeyHolder[3];
	int i=0;

	i=0;
	fp = fopen(key,"r");
	if ( fp == NULL ) { printf("Couldn't open key.\n"); return; }
	buffer = (char *)malloc(buffersize * sizeof(char));
        chars = getline(&buffer,&buffersize,fp);
//        printf("pubkey: %s\n",buffer);
        token = strtok(buffer," ");
        while( token != NULL ){
                pubKeyHolder[i] = atoll(token);
//		printf("%llu \n", pubKeyHolder[i]);
		i++;
		token = strtok(NULL, " ");
        }
        fclose(fp);

	i=0;
        fp = fopen(key,"r");
        buffer = (char *)malloc(buffersize * sizeof(char));
        chars = getline(&buffer,&buffersize,fp);
//      printf("Privkey: %s\n",buffer);
        token = strtok(buffer," ");
        while( token != NULL ){
		privKeyHolder[i] = atoll(token);
//		printf("%llu \n", privKeyHolder[i]);
		i++;
                token = strtok(NULL, " ");
        }
//        printf("\n");
        fclose(fp);

	// privkey -> p g d
	// pubkey -> p g e2
	unsigned long long int p = pubKeyHolder[0];
	unsigned long long int g = pubKeyHolder[1];
	unsigned long long int d = privKeyHolder[2];
	unsigned long long int e2 = pubKeyHolder[2];
//	printf("%llu %llu %llu %llu\n", p, g, d, e2);
	unsigned long long int c1, c2;
        unsigned long long int m;
	// Modulo is 32 bits, block size is 31 bits.
	// a character is 8 bits, so we pull 4 characters in
	fp = fopen(inputFile,"r");
	outfp = fopen(outputFile,"w");
	if ( fp == NULL ) { printf("Couldn't open input file.\n"); return; }
	if ( outfp == NULL ) { printf("Couldn't open output file.\n"); return; }
	char buf[10];
	long int k;
	int read = fread(&m, 1, 4, fp);
	while( read == 4 ){
		k = (rand() % (((p-1) + 1) - 1)) + 1 ;
		c1 = exponentiation(g,k,p);
		c2 = ((m%p) * (exponentiation(e2,k,p))) % p;	
//		printf("%llu - %llu %llu\n",m, c1, c2);
		fprintf(outfp, "%llu %llu\n", c1, c2);
		m = 0x0;
		read = fread(&m, 1, 4, fp);
	}
	if ( read > 0 ){
		k = (rand() % (((p-1) + 1) - 1)) + 1 ;
                c1 = exponentiation(g,k,p);
		c2 = ((m%p) * (exponentiation(e2,k,p))) % p;
                m = 0x0;
		fprintf(outfp, "%llu %llu\n", c1, c2);
                read = fread(&m, 1, 4, fp);
	}
	fclose(fp);
	fclose(outfp);
	printf("Done encrypting.\n");
	return;
}
