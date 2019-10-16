#include "mylib.h"

node* search(node*root,int number){

	if ( root == NULL ){
		 return(0);
	}

	if ( root->data == number ){
		return(root);
	}

	if ( root->data < number ){
		return(search(root->right,number));
	}
	if ( root->data > number ){
		(search(root->left,number));
	}


}
