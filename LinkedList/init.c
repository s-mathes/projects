#include "files.h"
#include <stdio.h>
//function that initializes the first 100 elements in a struct node
//node includes three fields: int data, int next, int valid
//init() sets up LL[0] as the sentinel node, which points at first item in list
//LL[0].next=MYNULL, which means it doesn't point to another item
//LL[0].valid=1 because it's flagged as in use
//all the valid fields of the other items are set to 0 to flag them as not in use
//function accepts one argument: the array of 100 struct nodes to be initialized

void init(struct node *myNode){
	int i=0; //index variable
	while ( i != 100 ){  //perform this 100 times
		if ( i == 0 ){ //for element[0], set valid=1,data=-98686, and next=-1
			myNode[i].data=-98686;
			myNode[i].next=-1;
			myNode[i].valid=1;
			i++; //increment to next element
		}
		else { //for elements 1-99, valid=0,data=999,next=-99999
			myNode[i].data=999;
			myNode[i].next=-99999;
			myNode[i].valid=0;
			i++; //move to next element
		}
	} //end main while loop
}
