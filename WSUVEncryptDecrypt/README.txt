Sarah Robison-Mathes
Project 1
Encryption/Decryption
sarah.robison-mathes@wsu.edu

TO COMPILE:
Run the command 'make' to execute the Makefile.

TO RUN PROGRAM:
Execute the following command for encryption:
	./main -e -k <key.txt> -in <plaintext.txt> -out <ciphertext.txt>
	
Execute the following command for decryption:
	./main -d -k <key.txt> -in <ciphertext.txt> -out <decrypted.txt>
	
There are no default text files hardcoded into the program, but it will generate the
file on the -out switch.

FILES:
f.c - Implements the F function that was described in the project description.
g.c - Implements the G function that was described in the project description.
hex_to_bin.c - Converts a single hex char to a binary representation.
hex_to_dec.c - Converts a single hex char to an integer.
hex2_to_dec.c - Converts a hex string 2 characters long to a decimal integer.
hex4_to_dec.c - Converts a hex string 4 characters long to a decimal integer.
hex8_to_dec.c - Converts a hex string 8 characters long to a decimal integer.
hex16_to_dec.c - Converts a hex string 16 characters long to a decimal integer.
k.c - Implements the K function that was described in the project description.
main.c - The main driver of the program.
mylib.h - Contains #includes and function prototypes.
plaintext_to_hex.c - Converts ASCII values to hex strings.
Makefile


