#include <stdio.h>
#include "files.h"
//function get_node() searches a struct node
//which searches for the first node where valid=0
//return the value of that index, or if no node is found, return -1
//accepts one argument: the struct node linked list that is being searched

int get_node(struct node*LL){
	int i=0;
	
	while ( i != 100 ) { //perform for 100 elements
		if ( LL[i].valid == 0 ){ //if value of the valid field is 0
			return(i); //leave function with value of that index
		}
		else {
			i++; //check next field
		}
	}
	return(-1); //if it leaves the while loop without returning, that means there was no empty space
}
