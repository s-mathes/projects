/***********************************************************************
name: 	
	Spencer Ross
	Sarah Mathes
	Jakob Miner
	
Project 2 -- Egg Shell -- esh

description:	
	CS 460 ////Description HERE////

Objectives:	
	•	Your EggShell executable should be called esh.

	•	esh should run an infinite loop that provides the user with a 
		prompt. At this prompt, the user should be able to enter a 
		command which your shell should endeavor to execute.

	•	The first task will be reading in the command, and since some 
		commands may contain arguments you will have to read in the 
		entire line. For this, I would recommend looking into [GNU Read-
		line library]. 

	•	After the input is read in, the shell will need to verify that 
		the command entered is indeed a legal command. All commands must 
		be found on the path specified by the environment variable, PATH. 

	•	The PATH consists of a series of paths delimited by the ‘:’ 
		character. To show the value of PATH, type echo $PATH on the linux 
		command line.

	•	To access the directories included on the PATH, you might want to 
		investigate the functions getenv() and strtok(). If esh cannot find 
		the command on the PATH, then it should check the current directory 
		‘.’. If the command is still not found, then it should print an error 
		message and present a new prompt, i.e., esh should not crash.
/* Includes and definitions */
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#define FALSE 0
#define TRUE 1
#define SIZE 128
/**********************************************************************
./esh <arg1> &

    maybe put some usage here??
**********************************************************************/

int main(int argc, char const *argv[]) {
	char *token, *input, *pathEnviron = getenv("PATH");
	char** paths = (char **)malloc(SIZE*sizeof(char*));
	char** args = NULL;
	bool runInTheBackground = false;
	bool found = false;
	int status, numArgs = 0;
	int numPaths = 0; // number of directories on PATH
	struct dirent *de;
	DIR *dr;
	int h;
	char argumentOne[1];

	// Seperate each directory on PATH by ":"
	token = strtok(pathEnviron,":");
	while ( token != NULL ){ 
		//printf("%s\n",token);
		paths[numPaths] = (char *)malloc(strlen(token)*sizeof(char *));
		strcpy(paths[numPaths],token);
		numPaths++;
		token = strtok(NULL,":");
	}
	numPaths--;	
	
	// Print statement to confirm paths in path[]
	int j=0;

	while (1){
		//for(int k=0; k<numArgs; k++) free(args[k]);
		runInTheBackground = false;
		args = (char **)realloc(args,0);
		j=0;
		numArgs = 0;
		int i =0;
		found = false;

		input = readline("esh>");
		if (!strcmp(input, "") || !strcmp(input, "&")) continue;
		token = strtok(input, " ");
		while ( token != NULL ){
			args = (char **)realloc(args, sizeof(char **)*(numArgs+1));
			args[numArgs] = (char *)malloc(sizeof(char *)*strlen(token));
			strcpy(args[numArgs],token);
			numArgs = numArgs + 1;
			token = strtok(NULL, " ");
		}

		// Assess if the last character is & and make the last argument NULL for execv
        	if ( strcmp(args[numArgs-1],"&") == 0 ){
                //printf("RUN IN THE BACKGROUND\n");
                	runInTheBackground = true;
                	args[numArgs-1] = NULL;
        	} else {
                	args[numArgs] = NULL;
        	}

		if(strcmp(args[0], "exit") == 0) {
			for(int k=0; k<numArgs; k++) free(args[k]);
			for(int k=0; k<numPaths; k++) free(paths[k]);
			free(args);
			free(paths);
			exit(0);
		}

		// Open each directory and see if args[0] is in them.
		//char argumentOne[1];
		strcpy(argumentOne,"");
		// Open each directory and see if args[0] is in them.
		j=0;
		h=0;

		while( j<= numPaths ){
			//printf("Checking Path: %s\n", paths[j]);
			dr=opendir(paths[j]);
			if(dr == NULL){
				// printf("Couldn't open directory.\n");
				break;
				//continue;
			}
			while( (de = readdir(dr)) != NULL){
				if(strcmp(de->d_name,args[0]) == 0){
                        //printf("It's in here: %s\n", paths[j]);
                        strcat(argumentOne,paths[j]);
                        strcat(argumentOne,"/");
                        strcat(argumentOne,args[0]);
                        //printf("Final first argument for execv: %s %s\n", argumentOne,args[0]);
                        h=j;
                        j=numPaths + 1;
                        if(!fork()) {
                                execv(argumentOne,args);
                                perror("Can't exec\n");
                        }
                        if (!runInTheBackground) wait(&status);
                        found = true;
                        break;
                }

			}
			if(found) break; // if command found exececuted stop looking for it
			//rewinddir(dr);
			//closedir(dr);
			j++;
		}
		if(found == false) {
		    char* curr_path = ".";
		    if((dr = opendir(curr_path)) == NULL) {
		        //printf("Couldn't open directory.\n");
		        //continue;
			break;
		    }
		    while((de = readdir(dr)) != NULL) {
		        if(strcmp(de->d_name,args[0]) == 0) {
		            //printf("It's in here: %s\n", curr_path);
		            strcat(argumentOne,curr_path);
		            strcat(argumentOne,"/");
		            strcat(argumentOne,args[0]);
		            //printf("Final first argument for execv: %s %s\n", argumentOne,args[0]);
		            h=j;
		            j=numPaths + 1;
		            if(!fork()) {
		                    execv(argumentOne,args);
		                    perror("Can't exec\n");
		            }
		            if (!runInTheBackground) wait(&status);
		            found = true;
		            break;
		        } 
		    }
			if(found==false) printf("ERROR: %s not found!\n", args[0]);
		}
	}
	
	
	return 0;
}


