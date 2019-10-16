#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct item {
	int weight;
	int value;
	char name[32];
	int quantity;
	};

struct result {
	int value;
	int count[128];	
	};

typedef struct item
	item;

typedef struct result
	result;

int initializeInventory(FILE*);
int inventorySize(item*);
int maxVal(int,int);
int valueOf(item*,int); //returns the value of the items in a bag
result maxValue(int,int); 

result computedResults[1024]; //index of computed results, index by capacity
item inventory[128]; //selection of items to choose from
