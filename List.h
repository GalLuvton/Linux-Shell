#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*a list struct*/
typedef struct List
{
    char* name; 			/*each node holds a name and a value*/
    char* value;
    struct List* nextLink; 	/*pointer to next node*/
} List;

/*creates and sets the value of a new node*/
List* createNode(char* name, char* value, List* next);

/*deletes and frees memory of a node*/
void deleteNode(List* node);

/*find the value in the list*/
int searchInList(List* list, char* name);

/*get the value of the node with the corresponding name*/
char* valueOfName(List* list, char* name);

/*change the value of the i-th node*/
void changeListValueInLoc(List* list, char* name, char* value, int loc);

/*add name-value pair to list*/
List* addToList(List* list, char* name, char* value);

/*delete name-value pair to list*/
List* deleteFromList(List* list, char* name);

/*clear the list, freeing all memory allocated*/
void freeList(List* list);

/*print the contants of the list*/
void printList(List* list);