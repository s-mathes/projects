#include "mylib.h"
//print the given level of a tree
//used to print breadth first order

void print_level(node*root,int levels){

	if ( root == NULL ) return; 

        if ( first_print != 1 && root != NULL && levels == 1 ){ //comma formatting
		printf(", ");
        }
	
	if ( levels == 1 ){ //once levels is at one, start going back up and printing
		printf("%d", root->data);
		first_print=0;
		return;
	}
	else if ( levels > 1 ){ //go down left root first, decrement height count each time
		print_level(root->left,levels-1); 
		print_level(root->right,levels-1);
	}
}
