#include "mylib.h"

// testing
int main(int argc, char *argv[]){
	// DIR *thisDir;
	// struct dirent *dp;
	struct args *paths = (struct args *)malloc(sizeof(struct args));;
	struct stat sb;
	pthread_t newThread;
	char cwd[PATH_MAX], fullCWD[PATH_MAX];
	strcpy(fullCWD,"/");
	getcwd(cwd,sizeof(cwd));
	char backupDir[PATH_MAX];
	strcpy(backupDir,cwd);
	strcat(backupDir,"/.backup");
	
	//printf("Current working directory: %s\n", cwd);
	//printf("Back up directory:         %s\n", backupDir);

	if ( argc == 2 && (strcmp(argv[1], "-r") == 0) ){
	//	printf(">>>>>>>> RESTORE\n");
		strcpy(paths->fullInPath,cwd);
        	strcpy(paths->fullOutPath,backupDir);
        	pthread_create(&newThread,NULL,backupRestore,(void *)paths);
        	pthread_join(newThread,NULL);
		return 0;
	}
	

	// --------- Create the directory .backup/
	if ( stat(".backup/", &sb) == -1 ){
		mkdir(".backup/", 0700);
	//	printf("Creating directory .backup/\n");
	} else {
	//	printf("Directory .backup/ already exists.\n");
	}

	//thisDir = opendir(".");
	//while ( (dp = readdir(thisDir)) != NULL ){
	//	printf("%s, %d\n", dp->d_name, dp->d_type); // 8 is a regular file, 4 is a directory
	//}
	
	strcpy(paths->fullInPath,cwd);
	strcpy(paths->fullOutPath,backupDir);
	pthread_create(&newThread,NULL,backup,(void *)paths);
	pthread_join(newThread,NULL);

	// test update
	return 0;
}
