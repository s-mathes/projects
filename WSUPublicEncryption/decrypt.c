#include "mylib.h"

void decrypt(char *inputFile, char *outputFile, char *key){
        FILE *fp, *outfp;
        outfp = fopen(outputFile,"w");
	char *buffer, *token;
        size_t buffersize = 100;
        size_t chars;
	unsigned long long int c[2];
        unsigned long long int privKeyHolder[3];
        int i=0;
	char mbuf[4];
	unsigned long long int m;

	// privkey ----> p g d
        i=0;
        fp = fopen(key,"r");
        buffer = (char *)malloc(buffersize * sizeof(char));
        chars = getline(&buffer,&buffersize,fp);
//        printf("Privkey: %s\n",buffer);
        token = strtok(buffer," ");
        while( token != NULL ){
                privKeyHolder[i] = atoll(token);
//		printf("%i %llu ",i,privKeyHolder[i]);
                token = strtok(NULL, " ");
		i++;
        }
//	printf("\n");
        fclose(fp);

	char *line = NULL;
	size_t len = 0;
	
	ssize_t read;
	unsigned long long int p = privKeyHolder[0];
	unsigned long long int g = privKeyHolder[1];
	unsigned long long int d = privKeyHolder[2];
	i = 0;
	fp = fopen(inputFile, "r");
	if ( fp == NULL) { printf("Failed to open input file.\n"); return; }
	while ( (read = getline(&line, &len, fp)) != -1 ){
		token = strtok(line," ");
		i=0;
	        while( token != NULL ){ // Break it up into c1 and c2
        	        c[i] = atoll(token);
			i++;
			token = strtok(NULL, " ");
        	}
		m = (exponentiation(c[0],p-1-d,p)*(c[1]%p))%p;
//		printf("m: %llu --- ", m);
		mbuf[3] = (m & 0xff000000) >> 24; // pull out numbers and turn them into ascii codes.
		mbuf[2] = (m & 0x00ff0000) >> 16;
		mbuf[1] = (m & 0x0000ff00) >>  8;
		mbuf[0] = (m & 0x000000ff);
//		printf("%s\n", mbuf);		
	
		fputs(mbuf,outfp);

	}
	fclose(fp);
	fclose(outfp);

	printf("Done decrypting.\n");
	return;
}
