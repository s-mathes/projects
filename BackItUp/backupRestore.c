#include "mylib.h"


void *backupRestore(void *input){
	DIR *thisDir;
	// struct args *paths = (struct args *)input;
	struct dirent *dp;
	char tempInputPath[PATH_MAX], tempOutputPath[PATH_MAX];
	strcpy(tempInputPath, ((struct args*)input)->fullInPath);  // cwd
	char restoreDir[PATH_MAX];
	char backupDir[PATH_MAX];
	strcpy(backupDir,((struct args*)input)->fullOutPath); // backupDir is .backup/
	// fullOutPath is the only one that matters, the restore path is derived from the fullOutPath
	long t = 1;

	char *token;
	token = strtok(backupDir, "/");
	strcpy(restoreDir,"/");
	while ( token != NULL ){
		if ( strcmp(token,".backup") != 0 ){
			strcat(restoreDir,token);
			strcat(restoreDir,"/");
		}
		token = strtok(NULL,"/");
	}
	strcpy(backupDir,((struct args*)input)->fullOutPath);

	strcpy(tempInputPath,backupDir);	
	strcpy(tempOutputPath, restoreDir); 
	struct args *copyPaths = (struct args *)malloc(sizeof(struct args));;
	pthread_t newThread, copyThread;
	thisDir = opendir(((struct args*)input)->fullOutPath);
        while ( (dp = readdir(thisDir)) != NULL ){
		if ( dp->d_type == 8 ){
			strcat(tempOutputPath,dp->d_name);
			tempOutputPath[strlen(tempOutputPath) - 4] = '\0';
			strcat(tempInputPath,"/");
			strcat(tempInputPath,dp->d_name);
			strcpy(((struct args*)copyPaths)->fullInPath, tempInputPath);
			strcpy(((struct args*)copyPaths)->fullOutPath, tempOutputPath);
			((struct args*)copyPaths)->threadid = t;
			t++;			
			pthread_create(&copyThread,NULL,copyFileRestore,(void *)copyPaths);
			pthread_join(copyThread,NULL);
			memset(tempInputPath,0,strlen(tempInputPath));
			strcpy(tempInputPath,backupDir);
			memset(tempOutputPath,0,strlen(tempOutputPath));	
			strcpy(tempOutputPath,restoreDir);
		}
		if ( dp->d_type == 4 && strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0 && strcmp(dp->d_name,".backup") != 0  ){
			strcat(tempOutputPath,dp->d_name);
			mkdir(tempOutputPath, 0777);
			strcpy(((struct args*)copyPaths)->fullOutPath,backupDir);
			strcat(((struct args*)copyPaths)->fullOutPath, "/");
			strcat(((struct args*)copyPaths)->fullOutPath,dp->d_name);
			strcpy(((struct args*)copyPaths)->fullInPath, restoreDir);
			strcat(((struct args*)copyPaths)->fullInPath,dp->d_name);
			pthread_create(&newThread,NULL,backupRestore,(void *)copyPaths);
			pthread_join(newThread,NULL);
			memset(tempInputPath,0,strlen(tempInputPath));
			strcpy(tempInputPath,backupDir);
			memset(tempOutputPath,0,strlen(tempOutputPath));
			strcpy(tempOutputPath,restoreDir);
		} 
	}

	return NULL;
}
