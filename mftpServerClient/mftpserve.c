#include "mftp.h"

int main(){
	struct sockaddr_in clientAddr;
	struct sockaddr_in servAddr; 
	struct sockaddr_in zeroedServAddr;
	int listenfd;
	int connectfd; // file descriptor to send data on
	int waitError; // checks for error when waiting for child processes.
	int	clientPID; // keep track of client processes
	int length;
	int status;
	
	// MAKE A SOCKET
	// AF_INET is the domain (internet)
	// SOCK_STREAM is the protocol family
	// if socket() < 0, there is an error
	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		perror("Socket");
		return(2);
	}
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

	// BIND THE SOCKET TO A PORT
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(MY_PORT_NUMBER);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	length = sizeof(struct sockaddr_in);

	if ( bind( listenfd,(struct sockaddr *) &servAddr, sizeof(servAddr)) < 0 ){
			perror("bind");
			exit(1);
	}
	
	listen(listenfd, 4); // permit a queue of four simultaneous connections, per writeup instructions
	// blocks until a connection is established by a client
	// returns a new descriptor for the connector
	// if accept() < 0, error
	// client's address is written into the sockaddr struct 
	
	while (1){ // server loop
        char buffer[8]; // buffer for sending server response to client.
		
		if ( (connectfd = accept(listenfd,(struct sockaddr*) &clientAddr,&length)) < 0){
				printf("%s\n", strerror(errno));
				return(2);
			}
		
		clientPID = fork();
		if ( clientPID < 0 ){ 
			printf("Fork() error: '%s'", strerror(errno));
			fflush(stderr);
		}
		
		if ( clientPID == 0 ){ // client
			char command;
			int hostEntry;
			char hostName[NI_MAXHOST];
			int datalistenfd; // listens for data connection.

			hostEntry = getnameinfo( (struct sockaddr*)&clientAddr, sizeof(clientAddr), hostName,sizeof(hostName),NULL,0,NI_NUMERICSERV);
			if ( hostEntry != 0 ){
				printf("%s\n", gai_strerror(hostEntry) );
				exit(0);
			}
			
			printf("Child %d, Client hostname: %s\n", getpid(), hostName);
			printf("Child %d, Connection accepted from host.\n", getpid());
			fflush(stdout);

			int portNumber = 0; //port number to listen on.
			
			while ( 1 ){ // waits for messages from the client
				struct stat statbuf; // checks if file is directory (stat())
				int childPID2 = 0; // id of child for 'ls' command
				int datafd = 0; // file descriptor to pipe in data - used in ls, get, put
				int	datalistenfd;
				int bytesRead = 0; // bytes read (for read()).
				int fd; // file descriptor
				char c;
				char readBuffer;
				char *pathname;				
				char message[300];
				struct sockaddr_in servAddrData; // address for data connection
				
				// Read user input into buffer.
				while (bytesRead += read(connectfd, &c, 1) == 1){
					if (c == '\n'){
						break;
					} else {
						message[bytesRead-1] = c;
					}
				}
				message[bytesRead-1] = '\0'; // remove '\n' character and replace with '\0' terminator
				pathname = message + 1; // if there's a pathname, it's offset by one character
				
				switch (  message[0] ) { // message[0] is the command letter
					case 'L': // list server directory			
						datafd = accept(datalistenfd, (struct sockaddr *) &clientAddr, &length);
						childPID2 = fork(); // fork another process
						if ( childPID2  < 0 ){ // failed, send error 
							printf("Fork() error: %s", strerror(errno));
							fflush(stdout);
							exit(1);
						} 
						if (childPID2 == 0){
							dup2(datafd, 1); // duplicate stdout to the data connection 
							close(connectfd);
							close(datafd);							
							close(listenfd);
							execlp("/bin/ls", "ls", "-l", (char *) 0);
						} else {
							close(datafd);
							wait(NULL);
						}
						write(connectfd, "A\n", 2); // send an acknowledgement to the client
						break;
					case 'C': // change server directory
						if ( chdir(pathname) == -1){ // chdir failed, send error message
							printf("Child %d, cd error:%s\n", getpid(), strerror(errno));
							fflush(stdout);
							write(connectfd, "E", 1);
							write(connectfd, strerror(errno), strlen(strerror(errno)));
							write(connectfd, "\n", 1);
						} else { // Send acknowledgment to client.
							printf("Child %d: Changed directory to '%s'\n", getpid(), pathname);
							fflush(stdout);
							write(connectfd, "A\n", 2);
						}
						break;					
				case 'G': // get file										
					datafd = accept(datalistenfd, (struct sockaddr *) &clientAddr, &length);
					printf("Child %d, file: %s\n", getpid(), pathname);
					fflush(stdout);
					
					if ( (fd = open(pathname, O_RDONLY )) < 0 ){
						write(connectfd, "E", 1); // let the client know that there was an error
						write(connectfd, strerror(errno), strlen(strerror(errno))*sizeof(char));
						write(connectfd, "\n", 1);
					} else if (stat(pathname, &statbuf) == 0){ // check if file is directory
						if ( S_ISDIR(statbuf.st_mode) ){ // if directory, send error to client
							write(connectfd, "E", 1);
							write(connectfd, "Error, file is a directory", 26);
							write(connectfd, "\n", 1);
						} else { 
							write(connectfd, "A\n", 2);
							int error;
							printf("Child %d: transmitting file %s to client\n", getpid(), (pathname));
							fflush(stdout);
							while ( (error = read(fd, &readBuffer, 1)) != 0){
								if ( error < 0){
										printf("Read() error: %s\n", strerror(errno));
										fflush(stdout);
										break;
								}
								if ( write(datafd, &readBuffer, 1) < 0 ){
									break;
								}
							}
						}
					}
					close(fd);
					close(datafd);
					break;
			
				case 'D': // initialize a data connection
					
					if ( (datalistenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
                		perror("Socket");
						return(2);
					}
					
					memset(&servAddrData, 0, sizeof(servAddrData));
					servAddrData.sin_family = AF_INET;
					servAddrData.sin_port = htons(0); // use port # 0, as recommended in writeup
					servAddrData.sin_addr.s_addr = htonl(INADDR_ANY);

					if ( bind( datalistenfd, (struct sockaddr *) &servAddrData, sizeof(servAddrData)) < 0){
						perror("bind");
						exit(1);
					}
					listen(datalistenfd, 4); // allow 4 simultaneous connections

					// this bit was instructed in the writeup to generate an ephemeral port
					memset(&zeroedServAddr, 0, sizeof(zeroedServAddr));
					getsockname(datalistenfd, (struct sockaddr *) &zeroedServAddr, &length);
					portNumber = ntohs(zeroedServAddr.sin_port);
					
					sprintf(buffer, "A%d\n", portNumber); // format the port number in a string					
					write(connectfd, buffer, 7); // and send it to the client, which will connect on it.
					break;
				case 'P': // Puts file into client dir.	
					datafd = accept(datalistenfd, (struct sockaddr *) &clientAddr, &length); // hook up data connection
					char filebuffer;
					char *filename;
					char *temp;
					
					// keep pulling out tokens until it gets to the name of the file
					filename = strtok(pathname, "/");
					do {
						temp = filename;
					} while ( (filename = strtok(NULL, "/") ) != NULL);

					printf("Child %d writing to file %s\n", getpid(), temp);
					fflush(stdout);

					if ((fd = open(temp, O_APPEND | O_WRONLY | O_EXCL | O_CREAT, 0666)) < 0){
						write(connectfd, "E", 1); // notify client of error
						write(connectfd, strerror(errno), strlen(strerror(errno))*sizeof(char));
						write(connectfd, "\n", 1);
					} else {
						write(connectfd, "A\n", 2); 
						printf("Child %d is receiving file %s\n", getpid(), temp);
						fflush(stdout);
						while ( read(datafd, &filebuffer, 1) > 0){ // while there is data to read,
							write(fd, &filebuffer, 1); // write it to the file descriptor.
						}
					}
					close(fd);
					close(datafd);
					break;
				
				case 'Q': // client is quitting
					printf("Child %d is exiting.\n", getpid());
					write(connectfd, "A\n", 2);
					close(connectfd);
					close(listenfd);
					message[0]='Z'; // reset message[0]
					break;
				default: 			
					while ((waitError = waitpid(clientPID, &status, WNOHANG)) > 0){
						if ( waitError == -1 ){
							printf("Error: '%s'", strerror(errno));
						}
					}
					close(connectfd);
				}
			}
		}
	}

	return 0;
}

