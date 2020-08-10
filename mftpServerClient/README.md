Final project in Systems Programming.

The miniature ftp system consists of two programs (executables),mftpserve
and mftp, the server and client respectively.
Both programs are written in C, to execute in Linux OS environment.
The  programs  utilize  the  Unix  TCP/IP  sockets  interface  (library)  
to  communicate  with  each  other  across  a TCP/IP network.  Only IPv4 addressing is supported by these programs.

The user initiates the client program using its name (mftp) with one required command line argument 
(in addition to the name of the program).  This argument will be interpreted by the client program as 
the name of the host on which the mftpserve process is running and to which the client will connect. 
The hostname may be either a symbolichost name or IPv4 dotted-decimal notation. 

Server and client send signals to each other as was detailed in project assignment write-up.
