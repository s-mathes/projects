#include "mylib.h"

int initializeInventory(FILE* fp){

	int i=0;
	char buffer[228];

        while ( fgets(buffer,sizeof(buffer),fp) != NULL ){  //complete until EOF
                sscanf(buffer,"%d %d %s",&inventory[i].weight,&inventory[i].value,inventory[i].name); //parse with sscanf
                i++; //increment to next item
        }

	fclose(fp);
	return(i-1);
}
