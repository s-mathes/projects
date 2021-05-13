#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <errno.h>


// FTable[rows][columns]
unsigned int g(unsigned int r0, int k, int k0, int k1, int k2, int k3,int round);
//
void rotate_right(char* string,char* result);
void rotate_left(char* string,char* result);
void rotate_key_left(char* binkey, int num);
int k(unsigned long long int key, int x);
int f(int r0,int r1, unsigned long long key, int round, char mode);
void plaintext_to_hex(char* plaintext_file, char* hex_file);
// takes a hex character and converts it to a decimal int.
// intended to index to ftable.
int hex_to_dec(char ch);
int hex2_to_dec(char* ch);
int hex4_to_dec(char* ch);
unsigned long long int hex16_to_dec(char *hex);
unsigned int hex8_to_dec(char* ch);
// Convert a hex char to a four digit binary.
void hex_to_bin(char ch, char* hex);

// convert a four digit binary string to a hex character
char bin_to_hex(char* bin); //, char* hex);

// xor 16 binary bits at a time
void xor(char* one, char* two, char* final);
