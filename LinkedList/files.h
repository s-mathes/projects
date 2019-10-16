struct node {
	int data;
	int next;
	int valid;
	};

typedef struct node
	node;

#define MYNULL -1

void init(struct node *); //initializes LL, sets up sentinel node
int get_node(struct node*); //return value is the index of next free node. if none, returns -1
int search(struct node*,int); //return 1 if number is in list, 0 if it's not
int add(struct node*LL,int number); 
void print(struct node*); //prints all elements in the LL in ascending order of data
void release_node(struct node*LL,int number); //called to clear the valid flag in the deleted node
int delete(struct node*LL, int); //delete that number from your linked list
