#include "mylib.h"

// struct stat->time_t st_mtime is the time of last modification

time_t getTime(char *pathname){

	struct stat filestat;

	stat(pathname, &filestat);
	//printf("Last modified time: %s", ctime(&filestat.st_mtime));

	return filestat.st_mtime;


}

char* stripPathname(char* fullInPath) {
	char delim[] = "/";
	char *last, *ptr = strtok(fullInPath, delim);

	while(ptr != NULL) {
		last = ptr;
		ptr = strtok(NULL, delim);
	}

	return last;
}
