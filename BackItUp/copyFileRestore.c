#include "mylib.h"


void *copyFileRestore(void *input){
	size_t bytes;
	long int bytesWritten = 0;

	char fileToCopy[PATH_MAX], backupFile[PATH_MAX], buffer[PATH_MAX];
	strcpy(fileToCopy,((struct args*)input)->fullInPath); // THE BACKUP FILE
	strcpy(backupFile, ((struct args*)input)->fullOutPath); // THE FILE TO BE RESTORED

	char ftcCopy[strlen(fileToCopy)], backupfCopy[strlen(backupFile)];
	strcpy(ftcCopy, fileToCopy);
	strcpy(backupfCopy, backupFile);
	
	char *justFilename = stripPathname(ftcCopy);
	char *justBackup = stripPathname(backupfCopy);
	FILE *inFP, *outFP, *fileCheck;
	//printf("Writing %s to %s\n", fileToCopy, backupFile);
	int fileExists;
	long t;
	t = ((struct args*)input)->threadid;
	
	fileCheck = fopen(backupFile,"r"); // opens a file for reading, but it must exist
	if (fileCheck == NULL) {
		//printf("Original file %s doesn't exist: %s\n",backupFile, strerror(errno));
		fileExists = 0;
	} else {
		//printf("Original file %s exists.\n", backupFile);
		fileExists = 1;
	}

	inFP = fopen(fileToCopy, "rb"); // READING FROM THIS FILE
	if (inFP == NULL) {
		printf("Error opening input %s: %s\n",justFilename, strerror(errno));
	}
	
	if ( fileExists == 1 && difftime(getTime(fileToCopy),getTime(backupFile)) < 0 ){ // backup file is newer than original 
		outFP = fopen(backupFile, "wb+");
		if ( outFP == NULL){; // Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file
			printf("Error opening output: %s\n", strerror(errno));
		}	
		printf("[Thread %ld] Restoring %s\n", t, justFilename);
		printf("[Thread %ld] WARNING: Overwriting %s\n", t, justFilename);
		while ( (bytes = fread(buffer, 1, sizeof(buffer), inFP)) > 0 ){
			if ( fwrite(buffer,1,bytes,outFP) != bytes) {
				// printf("in while: Wrote %ld bytes.\n", bytes);
			}
			bytesWritten = bytesWritten + bytes;
		}
		printf("[Thread %ld] Copied %ld bytes from %s to %s.\n", t, bytesWritten, justFilename, justBackup);
		fclose(inFP);
		fclose(outFP);
	} else if ( fileExists == 1 && difftime(getTime(fileToCopy),getTime(backupFile)) > 0 ) {
		printf("[Thread %ld] %s is already the most current version.\n", t, justBackup);
		fclose(inFP);
	}

	if ( fileExists == 0 ){ // the file doesn't exist, it needs to be backed up.
		outFP = fopen(backupFile, "wb+");
		if ( outFP == NULL){; // Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file
			printf("Error opening new file: %s\n", strerror(errno));
		}
		printf("[Thread %ld] Restoring %s\n", t, justFilename);
		while ( (bytes = fread(buffer, 1, sizeof(buffer), inFP)) > 0 ){
			if ( fwrite(buffer,1,bytes,outFP) != bytes) {
				//printf("in while: Wrote %ld bytes.\n", bytes);
			}
			bytesWritten = bytesWritten + bytes;
		}
		printf("[Thread %ld] Copied %ld bytes from %s to %s.\n", t, bytesWritten, justFilename, justBackup);
		fclose(inFP);
		fclose(outFP);
	}

	return NULL;
}
