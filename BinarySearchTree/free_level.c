#include "mylib.h"
//same algorithm as print_level, except it frees the memory
//of the nodes in a given level

void free_level(node*root,int levels){

        if ( root == NULL ) return;

        if ( levels == 1 ){
		free(root);
		return;
	} 

        else if ( levels > 1 ){ //left root first, decrement height count each time
                free_level(root->left,levels-1);
                free_level(root->right,levels-1);
        }

}
