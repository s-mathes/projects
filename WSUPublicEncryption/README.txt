Sarah Robison-Mathes
sarah.robison-mathes@wsu.edu
CS 427, Project 2
WSU-Pub-Crypt

To compile, run command 'make'.

To execute, perform the following commands:
./wsu-pub-crypt -genkey						GENERATE KEYS
./wsu-pub-crypt -e -k pubkey.txt -in ptext.txt -out ctext.txt 	ENCRYPT
./wsu-pub-crypt -d -k prikey.txt -in ctext.txt -out dtext.txt	DECRYPT

This program generates public keys which are stored in pubkey.txt and prikey.txt, and then encrypts
and decrypts a given text using a Miller-Rabin algorithm.
