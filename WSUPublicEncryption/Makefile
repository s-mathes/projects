wsu-pub-crypt: main.o 32_rand.o decrypt.o encrypt.o generate.o getq.o exponentiation.o witness.o miller_rabin.o
	gcc -o wsu-pub-crypt main.o 32_rand.o decrypt.o encrypt.o getq.o generate.o exponentiation.o witness.o miller_rabin.o -lm

main.o: main.c mylib.h
	gcc -c main.c -lm

encrypt.o: encrypt.c mylib.h
	gcc -c encrypt.c -lm

decrypt.o: decrypt.c mylib.h
	gcc -c decrypt.c -lm

getq.o: getq.c mylib.h
	gcc -c getq.c -lm

32_rand.o: 32_rand.c mylib.h
	gcc -c 32_rand.c -lm

miller_rabin.o: miller_rabin.c mylib.h
	gcc -c miller_rabin.c mylib.h -lm

witness.o: witness.c mylib.h
	gcc -c witness.c -lm

exponentiation.o: exponentiation.c mylib.h
	gcc -c exponentiation.c -lm

generate.o: generate.c mylib.h
	gcc -c generate.c -lm
