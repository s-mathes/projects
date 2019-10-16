#include "mylib.h"
//returns a pointer to the node with the largest data

node* find_largest(node* root) 
{ 
    //loop down to find the largest value
	while ( root->right != NULL ){ //largest value will always be
		root=root->right;      //to the right of the given node
	}			       //with no node to it's right
	return(root); 
} 
