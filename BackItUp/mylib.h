// includes
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>

// defines
#ifndef 	PATH_MAX
#define 	PATH_MAX 4096 //needed for Mac :/shrug ??
#endif


// function prototypes
void* backupRestore(void *input);
void* copyFileRestore(void *input);
void* copyFile(void *input);
void* backup(void *input);
time_t getTime(char *pathname);
char* stripPathname(char *fullInPath);

// structures
struct args {
	char fullInPath[PATH_MAX];
	char fullOutPath[PATH_MAX];
	long threadid;
};

