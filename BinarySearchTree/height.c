#include "mylib.h"
//find the height of the tree

int height(node*root){

	if ( root == NULL ) return(0); //once a recursive call reaches the bottom, left
				       //or right will be initialized and starting counting
				       //up as it returns to the top of the tree 

	int left=height(root->left); //traverse left side
	int right=height(root->right); //traverse right side

	//select larger and increment the count
	if ( left > right ){
		return(left+1);
	}
	else return(right+1);


}
