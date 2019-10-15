#include <stdio.h>
#include "files.h"
// routine should be called to clear the valid flag in the deleted node
// should use when you delete a node

void release_node(struct node*LL,int number){
		LL[number].valid=0; //clear the valid flag at that index 
}
