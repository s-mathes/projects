#include "mylib.h"

void postorder(node*root){

	if ( root == NULL ) return;

	postorder(root->left); //go down the left side first
	postorder(root->right); //then the right side
        if ( first_print != 1){
		printf(", ");
        }
	printf("%d", root->data); //then print the data
	first_print=0;
}
