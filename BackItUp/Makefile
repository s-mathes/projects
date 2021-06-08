AFLAGS=-o BackItUp
AFILES=main.c backup.c copyFile.c helpers.c backupRestore.c copyFileRestore.c
AOBJS=main.o backup.o copyFile.o helpers.o backupRestore.o copyFileRestore.o
EFLAGS=-Wall -Werror -pedantic

BackItUp: main.o backup.o copyFile.o helpers.o backupRestore.o copyFileRestore.o
	gcc $(EFLAGS) $(AFLAGS) $(AOBJS) -pthread

all: main.c backup.c copyFile.c helpers.c backupRestore.c copyFileRestore.c mylib.h
	gcc $(EFLAGS) $(AFLAGS) $(AFILES) -pthread

copyFileRestore.o: copyFileRestore.c mylib.h
	gcc $(EFLAGS) -c copyFileRestore.c -pthread

backupRestore.o: backupRestore.c mylib.h
	gcc $(EFLAGS) -c backupRestore.c -pthread

main.o: main.c mylib.h
	gcc $(EFLAGS) -c main.c -pthread

backup.o: backup.c mylib.h
	gcc $(EFLAGS) -c backup.c -pthread

copyFile.o: copyFile.c mylib.h
	gcc $(EFLAGS) -c copyFile.c -pthread

helpers.o: helpers.c mylib.h
	gcc $(EFLAGS) -c helpers.c -pthread

clean:
	rm -f *.o && rm -f BackItUp

unbackup:
	rm -r ./.backup

backup:
	./BackItUp

restore:
	./BackItUp -r 