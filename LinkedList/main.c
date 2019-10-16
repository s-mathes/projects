//Sarah Robison-Mathes, CSE 222, Winter 2019 "INTRODUCTION TO DATA STRUCTURES"
//Program 1 
//A program that will implement a linked list
//define a struct node with three int fields: data, next, and valid
//main program should initialize the linked list (LL) and set up the interface for the user
//from the interface, the user can add or delete numbers from the list,
//search for a number, print the list, or exit the program
//typedef, structs, and function prototypes are stored in files.h.
// See details for functions in their respective .c file

#include <stdio.h>
#include "files.h"

int main(void){
	node LL[100]; //linked list
	char input[100]; //2 element array, stores user's input
	int number; //stores the number the user enters

	init(LL); //initialize LL and set up sentinel node
	while ( 1 == 1 ){ //eternal loop
		printf("> ");
		fgets(input,100,stdin); //gather user's input
		if ( 1 == sscanf(input,"i %d", &number) ){ //attempt to convert to format "i #". if successful, attempt to insert in list
			if ( search(LL,number) == 1 ){ //number is already in list
				printf("NODE IS ALREADY IN LIST\n"); 
			}
			else if ( get_node(LL) == -1 ){ //list is full
				printf("OUT OF SPACE\n");
			}
			else if ( search(LL,number) == 0 ){ //number wasn't in list
				add(LL,number);
				printf("SUCCESS\n");
			}	 
		}
		else if ( input[0] == 'p' ){ //if p, print list
			print(LL);
		}
		else if ( 1 == sscanf(input,"d %d", &number) ){ //if "d #", attempt to delete that #
			if ( search(LL,number) == 1 ){ //number is in list
				delete(LL,number);
				printf("SUCCESS\n");
			}
			else if ( search(LL,number) == 0 ){ //number isn't in the list
                                printf("NODE NOT FOUND\n"); 
			}
		}
		else if ( input[0] == 'x' ){ //exit the program
			return(0);
		}
		else if ( 1 == sscanf(input,"s %d", &number) ){   //run a search for the number
			if ( search(LL,number) == 1){ //number is in the list
				printf("FOUND\n");
			}
			else { 
				printf("NOT FOUND\n");
			}
		}
		else { //didn't enter a valid command, print synopsis of commands
			printf("INVALID ENTRY. PLEASE ENTER:\ni #	to enter a # into the list\np  	to print the list\ns #	to search the list for a #\nd #	to delete a number from the list\nx	to exit the program.\n");
		}
	} 
}
