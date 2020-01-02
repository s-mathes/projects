main: main.o trapezoid.o f.o f2.o simpsonsOne.o simpsonsThree.o Booles.o
	gcc -o main main.o trapezoid.o f.o f2.o simpsonsOne.o simpsonsThree.o Booles.o -lm

Booles.o: Booles.c mylib.h
	gcc -c Booles.c -lm

simpsonsThree.o: simpsonsThree.c mylib.h
	gcc -c simpsonsThree.c -lm

simpsonsOne.o: simpsonsOne.c mylib.h
	gcc -c simpsonsOne.c -lm

main.o: main.c mylib.h
	gcc -c main.c -lm

f2.o: f2.c mylib.h
	gcc -c f2.c -lm

f.o: f.c mylib.h
	gcc -c f.c -lm

trapezoid.o: trapezoid.c mylib.h
	gcc -c trapezoid.c -lm
