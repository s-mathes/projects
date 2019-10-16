#include "mylib.h"

int inventorySize(item*inven){

	int i=0;

	while ( inven[i].weight != 0 ){
		i++;
	}
	return(i-1); //subtract one because we want to use 0 as an element
	//ex) four items in the inventory is case[0], case[1], case[2], case[3]

}
