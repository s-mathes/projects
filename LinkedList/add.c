#include <stdio.h>
#include "files.h"
//function that inserts a number into the linked list
//accepts two arguments: the struct node list array, and the number to be added
//returns a 0 if list is empty, returns -1 if list if full
//error handling is in main program (no space available, number not in list)

int add(struct node*LL,int number){
	int previous,current; //variables to store index of current inspected node and previous inspected node
	int insert; //variable to store location of new node
	current=LL[0].next; //start at the sentinel node


	if ( LL[0].next == MYNULL ){ //list is empty, put it in the first node
		LL[1].data=number; //store number
		LL[1].valid=1; //update valid vield
		LL[1].next=MYNULL; //update new MYNULL
		LL[0].next=1; //point sentinel to new first element
		return(0); //leave function
	}
		
	while ( LL[current].next != MYNULL ){ //perform while loop until it inserts a number
		if ( number > LL[current].data ){ //if number is larger than current data field
			previous=current; //save current index as index of previous node
			current=LL[previous].next; //move to the node that is being pointed at
			}
		else if ( number < LL[current].data ){ //if number is smaller than current data field
			insert=get_node(LL); //find first available node, value of index of added node
			LL[insert].data=number;//put number in that data field
			LL[insert].valid=1;//set that node valid to 1 (in use)
			LL[insert].next=current; //needs to point at the index of the node to come after
			LL[previous].next=insert;  //previous node needs to point at index of insert node
			return(0); //leave function
		}
	}
	//if it exits the while loop, still needs to be checked against last element
	if ( number < LL[current].data ){ //if number is smaller than last element, it needs to go before it
		insert=get_node(LL); //find first available node
		LL[previous].next=insert; //points to index of new node
		LL[insert].next=current; //new node needs to point to next node
		LL[insert].data=number;
		LL[insert].valid=1;
	}
	else if ( number > LL[current].data ){ //else number is largest data and needs to go last
		insert=get_node(LL); //find first available node
		LL[current].next=insert; //point last node to new largest number
		LL[insert].data=number; 
		LL[insert].next=MYNULL; //update new MYNULL
		LL[insert].valid=1; //flag node as in use
	}

}
