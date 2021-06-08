#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <math.h>
#include <stdbool.h>

unsigned long long int g,p,d,e2;
void encrypt(char *inputFile, char *outputFile, char *key);
unsigned long long int getq();
void decrypt(char *inputFile, char *outputFile, char *key);
unsigned long long int rand32();
unsigned long long int miller_rabin(unsigned long long int n, int s);
bool witness(unsigned long long int d, unsigned long long int n);
unsigned long long int generate(void);
unsigned long long int exponentiation(unsigned long long int a, unsigned long long int b, unsigned long long int n);
