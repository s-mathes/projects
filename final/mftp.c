#include "mftp.h"

// creates a data connection between the client and the host
int initDataConn(int socketNum, char *hostname){
        char portNumbers[20];
        char c;

        write(socketNum, "D\n", 2); // signals a data connection to the server
		
		int length = 0;
		while ( read(socketNum,&c,1) == 1){
			if ( c == '\n'){
				break;
			} else {
				portNumbers[length] = c;
				length++;
			}
		}
		portNumbers[length] = '\0';
        // remove the 'A' from the beginning of the string
        char portName[20];
        int i=1;
        while ( portNumbers[i] != '\0' ){
                portName[i-1]=portNumbers[i];
                i++;
        }
        portName[i-1]='\0';

        // Make the connection
        int socketfd;
        struct sockaddr_in servAddr;

        struct addrinfo hints, *actualdata;
        memset(&hints, 0, sizeof(hints));
        int err;

        hints.ai_socktype = SOCK_STREAM;
        hints.ai_family = AF_INET;
        err = getaddrinfo(hostname,portName, &hints, &actualdata);
        if ( err != 0 ){
            fprintf(stderr,"Error: %s\n",gai_strerror(err));
			exit(1);
        }
        socketfd = socket(actualdata -> ai_family, actualdata -> ai_socktype, 0);

        connect(socketfd, actualdata -> ai_addr, actualdata -> ai_addrlen);
        return socketfd;

}

// handles the response from the server ('A'cknowledge or 'E'rror)
// Returns -1 if there's an error, 0 if successful
int serverResponse(int socketfd){	
	char responseBuffer[100]; // Stores server response
	char c;  
	int length = 0;  

	while (length += read(socketfd, &c, 1) == 1){
		if (c == '\n'){
			break;
		} else {
			responseBuffer[length - 1] = c;
		}
	}
	responseBuffer[length - 1] = '\0';
	
	if (responseBuffer[0] == 'E'){ // error
		fprintf(stderr, "Error response from server: %s\n", responseBuffer + 1);
		fflush(stdout);
		return(-1);
	}
	if (responseBuffer[0] != 'A'){ // if server sends weird response (usually NULL)
		fprintf(stderr, "Unknown server response. \n");
		fflush(stdout);
		return(-1);
	}
	return 0;
}

int main(int argc, char *argv[]){
	
	if (argc != 2){
		printf("> ./mftp <hostname | IP address>\n");
		exit(0);
	}

	int socketfd;
	int bytesRead; // number of bytes read when reading commands from server.
	struct sockaddr_in servAddr;
	char *buffer;
	struct addrinfo hints, *actualdata; 
	memset(&hints, 0, sizeof(hints)); 
	
	// SET UP THE ADDRESS OF THE SERVER
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_family = AF_INET; 
	
	// setupErr != 0 if there was an error
	int setupErr = getaddrinfo(argv[1],"49999",&hints,&actualdata); 
	if ( setupErr != 0 ){ 
		fprintf(stderr,"Error: %s\n",gai_strerror(setupErr)); 
		exit(1); 
	} 

	socketfd = socket(actualdata->ai_family, actualdata->ai_socktype, 0); 
	if ( socketfd < 0 ){ //~
		 printf("%s\n", gai_strerror(setupErr)); 
		exit(2); 
	} 

	// CONNECT TO THE SERVER 
	// file i/o equivalent of open() 
	// returns < 0 if error 
	if ( (connect(socketfd, actualdata->ai_addr,actualdata->ai_addrlen)) < 0 ){ 
		printf("%s\n", strerror(errno)); 
		exit(1);	
	} 

	printf("Connected to server -- %s\n", argv[1]);

	buffer = (char *) malloc(sizeof(char) * 200);

	while (1){
		// command[]: commands to the server, translated by client.
		char command[500];
		int childPID; // ID for command processes 
		char *token[2]; // token[0] = command; token[1] etc = arguments.
		
		printf("> ");
		fgets(buffer, 300, stdin);

		if ( strcmp(buffer,"\n") == 0 ){
			printf("> ");
			fgets(buffer,300,stdin);
		}
		if ( strcmp(buffer,"\n") == 0 ){
			printf("Error: Please enter command and arguments.\n");
			exit(0);
		}

		// use strtok to separate the commands and arguments
		if ( (token[0] = strtok(buffer, " \t\r\n\v\f")) == 0){ // separate on white space
			// ... do nothing 
		} else {
			token[1] = strtok(NULL, " \t\r\n\v\f"); // token [0] is the command 
		}
		
		// process the command
		if (strcmp(token[0], "cd") == 0){ //--------------------- change directory
			if (token[1] == NULL){ // missing a pathname
				printf("Error: expecting a pathname.\n");
			} else {
				int errCheck = chdir(token[1]); // change directory and check for error
				if ( errCheck == -1 ){ 
					printf("Error: %s\n", strerror(errno));
				}
			}
			continue;
		} else if (strcmp(token[0], "rcd") == 0){ //---------------- change the server directory
			if (token[1] == NULL){ // assert an argument
				printf("Error: no path given.\n");
			} else { // send a message to the server, formatted as a string with the pathname.
				memset(command,0,strlen(command)); // reset the command string
				command[0]='C';
				strcat(command,token[1]);
				strcat(command,"\n");
			}
			if ( write(socketfd, command, strlen(token[1]) + 2) == -1){ // +2 for 'C' and '\n'
				printf("Error: '%s'\n", strerror(errno));
				exit(1);
			}
			serverResponse(socketfd);
		} else if (strcmp(token[0], "ls") == 0){ //---------------------- list local directory
			childPID = fork();
			if ( childPID  < 0){
				printf("Fork error: %s\n", strerror(errno));
				exit(1);
			} else if (childPID == 0){ // child
				int fd[2]; // fd[0] is the read end, fd[1] is the write end
				int childPIDls; // child PIDs inside ls
								
				int pipeErr = pipe(fd);
				if ( pipeErr < 0 ){
					printf("Pipe error: %s\n", strerror(errno));
					exit(1);
				}
				close(socketfd);
				
				childPIDls = fork();
				if ( childPIDls < 0){
					printf("Fork error: %s\n", strerror(errno));
					exit(1);
				} else if (childPIDls == 0){ // parent
					close(fd[0]); // close read end
					dup2(fd[1], 1); // duplicate write end to stdout
					close(fd[1]);
					if ( execlp("/bin/ls", "ls", "-l", (char *) 0) == -1){ // '-l' per write up instructions
						printf("%s\n",strerror(errno));
					}
				}  else { // child
					close(fd[1]);
					dup2(fd[0], 0);
					close(fd[0]); // duplicate read end to stdin
					if ( (execlp("/bin/more", "more", "-20", (char *) 0)) == -1){
						printf("%s\n",strerror(errno));
					}
				} 
			}
			wait(NULL);
		} else if (strcmp(token[0], "rls") == 0){ //---------- show server directory
			int dataConnectionFD = initDataConn(socketfd, argv[1]); // establish a data connection

			if ( write(socketfd, "L\n", 2) == -1 ){
				fprintf(stderr, "Error: writing to server - error# %d -- %s\n", errno, strerror(errno));
				exit(1);
			}
			
			if ( serverResponse(socketfd) == -1){ // if error, stop 
				continue;
			}
			
			childPID = fork(); 
			if ( childPID  < 0){
				fprintf(stderr, "Error: error using fork() -- %d: %s\n", errno, strerror(errno));
				exit(1);
			} else if (childPID == 0){
				dup2(dataConnectionFD, 0);
				close(dataConnectionFD);
				close(socketfd);
				execlp("/bin/more", "more", "-20", (char *) 0); // more -20 per the writeup recommendation
			}

			close(dataConnectionFD);
			wait(NULL);
		} else if ( strcmp(token[0], "get") == 0 ){ //------------------- get file
			int fd; // file descriptor for file to write to
			char c;
			char *filename, *temp;
			if ( token[1] == NULL ){ // no path was given
				printf("Error: expecting a pathname.\n");
				continue;
			}
			filename = strtok(token[1], "/");
			
			do { // Get filename from path.
				temp = filename;
			} while ((filename = strtok(NULL, "/")) != NULL);	
			
			if ( open(temp, O_WRONLY, 0666) > 0){ // file already exists
				printf("File exists.\n");
				continue; // end operation if file cannot be opened
			}
	
			int dataConnectionFD = initDataConn(socketfd, argv[1]);
			memset(command,0,strlen(command)); // reset the command string
			command[0]='G'; // send 'get' signal to server
			strcat(command,token[1]);
			strcat(command,"\n");
			
			if ( write(socketfd, command, strlen(token[1])+2) == -1){ // +2 for 'G' and '\n'
				printf("Write error: %s\n", strerror(errno));
				exit(0);
			}
			
			if ( serverResponse(socketfd) == -1){ // error check from server
				continue;
			}
			
			fd = open(temp, O_CREAT | O_APPEND | O_EXCL | O_WRONLY, 0666); // create file, point fd at it
			if ( fd < 0 ){ 
				printf("Error on creating file: %s\n", strerror(errno));
				close(fd);
				continue;
			}
			
			while ( read(dataConnectionFD, &c, 1) > 0){ // read data from the data connection as long as its there
				write(fd, &c, 1); // write it to the file descriptor
			}

			close(fd);
			close(dataConnectionFD);
		} else if (strcmp(token[0], "show") == 0){ //------------ show server contents
			if ( token[1] == NULL ){
				printf("Error: expecting a pathname.\n");
				continue;
			}
			int dataConnectionFD = initDataConn(socketfd, argv[1]);
			
			memset(command,0,strlen(command)); // reset the command string
			command[0]='G'; 
			strcat(command,token[1]);
			strcat(command,"\n");
			write(socketfd, command, strlen(token[1])+2); // send 'get' command to server
			
			if (serverResponse(socketfd) == -1){ // error check
				close(dataConnectionFD);
				continue;
			}	
			childPID = fork();
			if ( childPID < 0){
				printf("Fork() error: %s\n", strerror(errno));
				exit(1);
			} else if (childPID == 0){
				dup2(dataConnectionFD, 0);
				close(dataConnectionFD);
				execlp("/bin/more", "more", "-20", (char *) 0); // more -20 per writeup instructions
			}

			close(dataConnectionFD);
			wait(NULL);
		} else if ( strcmp(token[0], "put") == 0 ){ //---------------- put file command
			if (token[1] == NULL){
				printf("Error: expecting a pathname.\n");
				continue;
			}

			char readBuffer;
			struct stat	statbuf;
			int fd; // read() file descriptor
			int readError; 			
			
			if ( (fd = open(token[1], O_RDONLY)) < 0){ // error
				printf("Open() error: %s\n", strerror(errno)); 
			} else if ( stat((token[1]), &statbuf) == 0){ // check out the status of the pathname
				if ( S_ISDIR(statbuf.st_mode) ){ // error - it's a directory
					printf("Error: File is a directory.\n");
					close(fd);	
					continue;
				} else {
					int dataConnectionFD = initDataConn(socketfd, argv[1]);
					memset(command,0,strlen(command)); // reset the command string
					command[0]='P'; // send 'put' signal to server
					strcat(command,token[1]);
					strcat(command,"\n");
					
					if ( write(socketfd, command, strlen(token[1])+2) == -1){ // +2 for 'P' and '\n'
						printf("Write() error: %s\n", strerror(errno));
						exit(0);
					}
					
					if (serverResponse(socketfd) == -1){ // check for error from server
						close(dataConnectionFD);
						continue;
					}	

					while ( (readError = read(fd, &readBuffer, 1)) != 0 ){
						if (readError < 0){
							printf("Read() error: %s\n", strerror(errno));
							break;
						}
						if ( write(dataConnectionFD, &readBuffer, 1) < 0 ){
							printf("Write() error: %s\n", strerror(errno));
							break;
						}
					}
					close(fd);
					close(dataConnectionFD);
				}
			}
		} else if (strcmp(token[0], "exit") == 0 || strcmp(token[0],"quit") == 0 ){ // quit program
			write(socketfd, "Q\n", 2);
			break;
		} else {
			printf("Valid commands:\ncd <pathname>\nrcd <pathname>\nls\nrls\nget <pathname>\nshow <pathname>\nput <pathname>\n");
			continue;
		}
	}

	close(socketfd);
	return 0;
}






