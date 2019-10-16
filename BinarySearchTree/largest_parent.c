#include "mylib.h"
//finds the parent of the largest node in a tree
//did not end up using this in final project, may revisit later

node* largest_parent(node* root){

	node*previous;

	while ( root->right != NULL ){
		previous=root; //save parent
		root=root->right;		
	}

	return(previous); 

}
