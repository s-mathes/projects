#include "mylib.h"


void *copyFile(void *input){
	size_t bytes;
	long int bytesWritten = 0;

	char fileToCopy[PATH_MAX], backupFile[PATH_MAX], buffer[PATH_MAX];
	strcpy(fileToCopy,((struct args*)input)->fullInPath);
	strcpy(backupFile, ((struct args*)input)->fullOutPath);

	char ftcCopy[strlen(fileToCopy)], backupfCopy[strlen(backupFile)];
	strcpy(ftcCopy, fileToCopy);
	strcpy(backupfCopy, backupFile);
	
	char *justFilename = stripPathname(ftcCopy);
	char *justBackup = stripPathname(backupfCopy);
	FILE *inFP, *outFP, *fileCheck;
	//printf("Copying %s to %s\n", fileToCopy, backupFile);
	int fileExists;
	long tid;
	tid = ((struct args*)input)->threadid;
	
	fileCheck = fopen(backupFile,"r"); // opens a file for reading, but it must exist
	if (fileCheck == NULL) {
		// printf("Back up file %s doesn't exist: %s\n",backupFile, strerror(errno));
		fileExists = 0;
	} else {
		// printf("Back up file exists.\n");
		fileExists = 1;
	}

	inFP = fopen(fileToCopy, "rb");
	if (inFP == NULL) {
		printf("Error opening input %s: %s\n",justFilename, strerror(errno));
	}
	
	if ( fileExists == 1 && difftime(getTime(fileToCopy),getTime(backupFile)) > 0 ){ // back up exists and filetocopy is newer than backupfile and needs to be replaced.
		outFP = fopen(backupFile, "wb");
		if ( outFP == NULL){; // Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file
			printf("Error opening output: %s\n", strerror(errno));
		}	
		printf("[Thread %ld] Backing up %s\n", tid, justFilename);
		printf("[Thread %ld] WARNING: Overwriting %s\n", tid, justFilename);
		while ( (bytes = fread(buffer, 1, sizeof(buffer), inFP)) > 0 ){
			if ( fwrite(buffer,1,bytes,outFP) != bytes) {
				// printf("Wrote %ld bytes.\n", bytes);
			}
			bytesWritten = bytesWritten + bytes;
		}
		printf("[Thread %ld] Copied %ld bytes from %s to %s\n", tid, bytesWritten, justFilename, justBackup);
		fclose(inFP);
		fclose(outFP);
	} else if ( fileExists == 1 && difftime(getTime(fileToCopy),getTime(backupFile)) < 0 ){
		printf("[Thread %ld] %s does not need to be backed up.\n", tid, justFilename);
		fclose(inFP);
	}

	if ( fileExists == 0 ){ // the file doesn't exist, it needs to be backed up.
		outFP = fopen(backupFile, "wb");
		if ( outFP == NULL){; // Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file
			printf("Error opening output: %s\n", strerror(errno));
		}
		printf("[Thread %ld] Backing up %s\n", tid, justFilename);
		while ( (bytes = fread(buffer, 1, sizeof(buffer), inFP)) > 0 ){
			if ( fwrite(buffer,1,bytes,outFP) != bytes) {
				// printf("in while: Wrote %ld bytes.\n", bytes);
			}
			bytesWritten = bytesWritten + bytes;
		}
		printf("[Thread %ld] Copied %ld bytes from %s to %s\n",tid, bytesWritten, justFilename, justBackup);
		fclose(inFP);
		fclose(outFP);
	}

	return NULL;
}
