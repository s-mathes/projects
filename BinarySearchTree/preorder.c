#include "mylib.h"

void preorder(node*root){

	if ( root == NULL ) return;

	if ( first_print != 1){
		printf(", ");
	}
        printf("%d", root->data);
	first_print=0;
	preorder(root->left);
	preorder(root->right);

}
