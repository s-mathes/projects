name: 	Sarah Robison-Mathes
		Spencer Ross
		Jakob Miner

Files included:
	main.c  			- The main program reading command line
	backup.c 			- handles spinning up backup threads
	copyFile.c 			- handles copying files to backup dir
	helpers.c 			- two helper functions getTime() and stripFilename()
	backupRestore.c 	- handles spinning up restore threads
	copyFileRestore.c 	- handles copying files from backup dir to restore
	mylib.h 			- header file with includes, defines, prototypes and structs
	README.txt 				<--- you are here
	Makefile

////////////////////////////////////////////////////////////////////////
How to run:
	$ make 						//this will create object files
								//use "$ make all" to compile without .o files
	$ make backup 				//creates .bak files of current working dir
	  	OR $ ./BackItUp
	$ make restore 				// restores .bak files for files with changes
		OR $ ./BackItUp -r
	$ make unbackup 			// wipes out .backup/ directory
	$ make clean 				// wipes out all .o files and BackItUp executable
	
Execute with command './BackItUp' to backup files in .backup/ or './BackItUp -r' to restore files from .backup/ into working directory.
////////////////////////////////////////////////////////////////////////	

BackItUpshould creates a directory .backup/ if one does not already exist (Unix Tip: you must use the ls -a command to view this fiel).
BackItUpshould  then  creates  a  copy  of  all  the  regular  files  in  the  current  working directory in the .backup/ directory.  All the backup files  have .bak appended to their name.
If  a  backup  file  already  exists,  the  program  compares  the  last  modification times of the original file and the backup file and determine if a backup is required.  If the existing.bakbackup file is older, then the program overwrites it and  prints out a warning when doing so.  Otherwise, it will not overwrite the file, and will notify the user that the file is already the most current version.  
The program allocates a new thread to copy each file.  Therefore, if the current working directory contains five files, then the program will create five threads to backup each file.  The program will not terminate until all the threads have finished.
The program recursively  handle  subdirectories.   In  each  subdirectory,  it spawns one thread per file.  
If the user invokes BackItUpwith the optional -r(restore) argument, then it will restore  all  backup  files  in  the .backup/ directory  by  copying  them  to  the  current  working directory.  It will not restore any files that have a later modification time than the backed up copy.  Like before,  the restore function will create a separate thread foreach file being copied.
