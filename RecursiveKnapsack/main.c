/*
CSE 222 - Introduction to Data Structures
Clark College, Winter 2019
SARAH MATHES --- Coding 
KYLE STENNFELD -- Troubleshooting, Error checking
THOMAS BAILEY --- System design, Editing and optimization



----------STRUCTURE OVERHEAD------------

PARSE-->(1 ARG)-->(USE DEFAULT FILE)-\
     \				      -->(INITALIZE)-->(PRINT FILE INFO)-->(RECURSE)-->(PRINT SOLUTION)-->EXIT
      ->(2 ARG)-->(USE CUSTOME FILE)-/



*/

#include "mylib.h"

int main(int argc,char **argv){

	FILE *fp; //file pointer
	int size;
	int capacity;
	char file[250];
	int i=0;
	result finalResult;

//PARSE ARGV-->ERROR CHECK-->INITIALIZE INVENTORY
//---------------------------------------------------------------------------------------------------------
	if ( argc < 2 ){ 					//EDGE 1:no arguments passed, print error and quit
		printf("ERROR: please execute with a capacity value as the second argument\n");
		printf("Optional: location of inventory data file as the thrird argument.\n");
		return(0);					//Exit 
	}
	if ( argc == 2 ){ 					//capacity value passed, use default /tmp/knapsack.data
		fp=fopen("/tmp/knapsack.data", "r"); 		//point file pointer at default data file
		size=initializeInventory(fp); 			//intialize array of inventory items
		if ( sscanf(argv[1],"%d",&capacity) == 1 ){ 	//successfully parsed
			printf("Max capacity: %d\n",capacity);	//Print Error statement
		}
		else { //wasn't parsed, print error and quit
			printf("Please enter a valid number from 1 to 1024 as the second argument.\n");
			return(0); 
		}
		if ( capacity < 1 || capacity > 1024 ){ //capacity out of range, print error and quit
			printf("Please enter a valid number from 1 to 1024 as the second argument.\n");
			return(0);
		}
	}
	if ( argc == 3 ){ //third argument exists, use as custom inventory data file
		if ( sscanf(argv[2],"%s",file) == 1 ){ //successfully parsed into string
			while ( i <= strlen(file)-1 ){ //replace the newline string with a null terminator
				i++;
			}
			file[i]='\0'; 
			fp=fopen(file,"r");
			if ( fp == NULL ){ //invalid file, print error and quit
				printf("Invalid file.\n");
				return(0);
			}
			size=initializeInventory(fp);
			printf("file: %s\n",file);
	                if ( sscanf(argv[1],"%d",&capacity) == 1 ){ //successfully parsed
        	                printf("Max capacity: %d\n",capacity); //-----error checking print statement
               		 }
	                else { //wasn't parsed, print error and quit
	                        printf("Please enter a valid number from 1 to 1024 as the second argument.\n");
	                        return(0);
	                }
	                if ( capacity < 1 || capacity > 1024 ){ //capacity out of range, print error and quit
	                        printf("Please enter a valid number from 1 to 1024 as the second argument.\n");
	                        return(0);
	                }
		}
	}
//INVENTORY RECURSION SEARCH
//---------------------------------------------------------------------------------------------------------
        size=inventorySize(inventory); //will return one less than actual size because we want to use inventory[0]
	printf("no of items: %d\n",size+1);
	finalResult=maxValue(capacity,size);
//PRINT STATEMENTS
//---------------------------------------------------------------------------------------------------------
	i=0;
	while(i<=size){
		printf("weight=%d value=%d name=%s\n",inventory[i].weight,inventory[i].value,inventory[i].name);

	i++;
	}
	i=0;
	while(i<=size){
		printf("%s: %d\n",inventory[i].name, finalResult.count[i]);
		i++;
	}

	printf("highest value: %d\n",finalResult.value);

	return(0);
}	





//-------------------------error checking print statements
//	printf("weight=%d value=%d name=%s\n",inventory[0].weight,inventory[0].value,inventory[0].name);
//        printf("weight=%d value=%d name=%s\n",inventory[1].weight,inventory[1].value,inventory[1].name);
//        printf("weight=%d value=%d name=%s\n",inventory[2].weight,inventory[2].value,inventory[2].name);
//	printf("weight=%d value=%d name=%s\n",inventory[3].weight,inventory[3].value,inventory[3].name);
//	size=inventorySize(inventory); //will return one less than actual size because we want to use inventory[0]

