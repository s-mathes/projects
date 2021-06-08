#include "mylib.h"
//unsigned long long int g,p,d,e2;

unsigned long long int generate(void){
	// in the slides, e1 is the generator g
	char public_key_out[12] = "pubkey.txt"; // p g e2
	char private_key_out[12] = "prikey.txt"; // p g d
	unsigned long long int prime;
	bool isPrime = false;
	unsigned long long int q;
	time_t t;
	// Find  a safe 32-bit prime 
	int flag = 0;
	unsigned long long int g = 2;

	while ( 1 ){
		q = getq();
		//printf("\nq=%lli, trying to get p...\n",q);
		p = 2 * q + 1;
		//printf("p=%lli...\n",p);
		flag = miller_rabin(p,1000000);
		//printf("FLAG: %i\n",flag);
		if ( flag == 0 ) break;	
	}
//	printf("p = %lli\n",p);
	

	// Select d to be a member of {1...p-2}
	d = (unsigned long int)rand()%(p-2);
	// e1 and g are the same
	unsigned long long int e2 = exponentiation(g, d, p);
//	printf("g = %lli\n",g);
//	printf("d = %lli\n",d);	
//	printf("e2 = %lli\n",e2);

	// prikey is p g d
	FILE *fp;
	fp = fopen("prikey.txt", "w");
	fprintf(fp,"%lli %lli %lli",p,g,d);
	// pubkey is p g e2
	fclose(fp);
	fp = fopen("pubkey.txt", "w");
        fprintf(fp,"%lli %lli %lli",p,g,e2);
	fclose(fp);
	printf("Public key sent to: %s\n", public_key_out);
	printf("Private key sent to: %s\n", private_key_out);
	return q;
}
