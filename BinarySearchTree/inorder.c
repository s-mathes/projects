#include "mylib.h"

void inorder(node *root){

	if ( root == NULL ) return;

	if ( root != NULL ){
		inorder(root->left);
		if ( first_print != 1){
                	printf(", ");
	        }
		printf("%d", root->data);
		first_print=0;
		inorder(root->right);
	}

}
