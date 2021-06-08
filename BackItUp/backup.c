#include "mylib.h"


void *backup(void *input){
	DIR *thisDir;
	struct args *paths = (struct args *)input;
	struct dirent *dp;
	char tempInputPath[PATH_MAX], tempOutputPath[PATH_MAX];
	strcpy(tempInputPath, ((struct args*)input)->fullInPath);  // cwd
	char thisCWDir[PATH_MAX];
	char backupDir[PATH_MAX];
	strcpy(backupDir,((struct args*)input)->fullOutPath);
	strcpy(thisCWDir,((struct args*)input)->fullInPath);
	strcpy(tempOutputPath, ((struct args*)input)->fullOutPath); // .backup/

	struct args *copyPaths = (struct args *)malloc(sizeof(struct args));;
	pthread_t newThread, copyThread;
	long t = 1;

	thisDir = opendir(((struct args*)input)->fullInPath);
        while ( (dp = readdir(thisDir)) != NULL ){
		if ( dp->d_type == 8 ){
			//printf("%s, %d --> ", dp->d_name, dp->d_type); // 8 is a regular file, 4 is a directory
			strcat(tempOutputPath,"/");
			strcat(tempOutputPath,dp->d_name);
			//printf("going to: %s\n", tempOutputPath);
			strcat(tempInputPath,"/");
			strcat(tempInputPath,dp->d_name);
			strcpy(((struct args*)copyPaths)->fullInPath, tempInputPath);
			strcpy(((struct args*)copyPaths)->fullOutPath, tempOutputPath);
			strcat(((struct args*)copyPaths)->fullOutPath,".bak");
			((struct args*)copyPaths)->threadid = t;
			t++;
			pthread_create(&copyThread,NULL,copyFile,(void *)copyPaths);
			pthread_join(copyThread,NULL);
			//strcpy(((struct args*)paths)->fullOutPath,tempOutputPath);
			strcpy(tempOutputPath,"");
			memset(tempInputPath,0,strlen(tempInputPath));
			strcpy(tempInputPath,thisCWDir);
			memset(tempOutputPath,0,strlen(tempOutputPath));	
			strcpy(tempOutputPath,backupDir);
		}
		if ( dp->d_type == 4 && strcmp(dp->d_name,".") != 0 && strcmp(dp->d_name,"..") != 0 && strcmp(dp->d_name,".backup") != 0  ){
			strcat(tempInputPath,"/");
			strcat(tempInputPath,dp->d_name);
			strcpy(((struct args*)paths)->fullInPath,tempInputPath);
			strcat(tempOutputPath,"/");
			strcat(tempOutputPath,dp->d_name);
			// printf("----------- CREATE %s\n",tempOutputPath); // this is where this directory needs to be made in .backup
			mkdir(tempOutputPath, 0777);
			strcpy(((struct args*)paths)->fullOutPath,tempOutputPath);
			pthread_create(&newThread,NULL,backup,(void *)paths);
			pthread_join(newThread,NULL);
			memset(tempInputPath,0,strlen(tempInputPath));
			strcpy(tempInputPath,thisCWDir);
			memset(tempOutputPath,0,strlen(tempOutputPath));
			strcpy(tempOutputPath,backupDir);
		}
	}

        return NULL;
	// printf("----------- Completed navigation of %s\n", thisCWDir);
}
