main: main.o g.o f.o hex_to_dec.o hex16_to_dec.o hex4_to_dec.o hex2_to_dec.o k.o hex8_to_dec.o plaintext_to_hex.o
	gcc -o main main.o f.o g.o hex16_to_dec.o hex4_to_dec.o hex2_to_dec.o hex_to_dec.o k.o hex8_to_dec.o plaintext_to_hex.o -lm

plaintext_to_hex.o: plaintext_to_hex.c mylib.h
	gcc -c plaintext_to_hex.c -lm

main.o: main.c mylib.h
	gcc -c main.c -lm

g.o: g.c mylib.h
	gcc -c g.c -lm

hex16_to_dec.o: hex16_to_dec.c mylib.h
	gcc -c hex16_to_dec.c -lm

hex8_to_dec.o: hex8_to_dec.c mylib.h
	gcc -c hex8_to_dec.c -lm

hex4_to_dec.o: hex4_to_dec.c mylib.h
	gcc -c hex4_to_dec.c -lm

hex2_to_dec.o: hex2_to_dec.c mylib.h
	gcc -c hex2_to_dec.c -lm

f.o: f.c mylib.h
	gcc -c f.c -lm

k.o: k.c mylib.h
	gcc -c k.c -lm

hex_to_dec.o: hex_to_dec.c mylib.h
	gcc -c hex_to_dec.c -lm



