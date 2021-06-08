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
