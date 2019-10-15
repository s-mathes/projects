main: main.o push.o pop.o makeNFA.o makeTransition.o printNFA.o makeUnion.o isEmpty.o Kleene.o concatNFA.o
	gcc -o main main.o push.o pop.o makeNFA.o makeTransition.o printNFA.o makeUnion.o isEmpty.o Kleene.o concatNFA.o

main.o: main.c mylib.h
	gcc -c main.c

concatNFA.o: concatNFA.c mylib.h
	gcc -c concatNFA.c

Kleene.o: Kleene.c mylib.h
	gcc -c Kleene.c

isEmpty.o: isEmpty.c mylib.h
	gcc -c isEmpty.c

makeUnion.o: makeUnion.c mylib.h
	gcc -c makeUnion.c

print.o: print.c mylib.h
	gcc -c print.c

push.o: push.c mylib.h
	gcc -c push.c

pop.o: pop.c mylib.h
	gcc -c pop.c

makeNFA.o: makeNFA.c mylib.h
	gcc -c makeNFA.c

makeTransition.o: makeTransition.c mylib.h
	gcc -c makeTransition.c
