#include "mylib.h"
//add a number to the tree

node* add(node* root,int number){

	if ( root == NULL ){ //empty root, make it the first node
	        root=malloc(sizeof(node)); //allot space
	      	root->data=number; //assign data
	        root->left=NULL;
	        root->right=NULL; //point left and right child at nothing
		return(root); //return address of new node
	}
	else if ( root->data < number ){ //number is larger, dive to the right
		root->right=add(root->right,number);
		return(root);
	}
	else { //number is smaller, dive to the left
		root->left=add(root->left,number);
		return(root);
	}

}
