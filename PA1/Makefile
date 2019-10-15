main: main.o init.o add.o search.o delete.o get_node.o print.o release_node.o
	gcc -o main main.o add.o init.o search.o delete.o get_node.o print.o release_node.o

main.o: main.c files.h
	gcc -c main.c 

add.o: add.c files.h
	gcc -c add.c

init.o: init.c files.h
	gcc -c init.c

search.o: search.c files.h
	gcc -c search.c

delete.o: delete.c files.h
	gcc -c delete.c

get_node.o: get_node.c files.h
	gcc -c get_node.c

print.o: print.c files.h
	gcc -c print.c

release_node.o: release_node.c files.h
	gcc -c release_node.c

