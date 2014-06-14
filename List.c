#include "List.h"

/*creates and sets the value of a new node*/
List* createNode(char* name, char* value, List* next){
	char* newName;
	char* newValue;
	List* newNode;
	newName= (char*)malloc(strlen(name)+1);
	strcpy(newName, name);
	newValue= (char*)malloc(strlen(value)+1);
	strcpy(newValue, value);
	newNode= (List*)malloc(sizeof(List));
	newNode->name= newName;
	newNode->value= newValue;
	newNode->nextLink= next;
	return newNode;
}

/*deletes and frees memory of a node*/
void deleteNode(List* node){
	free(node->name);
	free(node->value);
	free(node);
}

/*find the value in the list*/
int searchInList(List* list, char* name){
	List* currentLink= list;
	int i= 0;
	while (currentLink){
		if(!strcmp(currentLink->name,name)){
			return i;
		}
		currentLink= currentLink->nextLink;
		i++;
	}
	return -1;
}

/*get the value of the node with the corresponding name*/
/*returns -1 if node does not exist*/
char* valueOfName(List* list, char* name){
	List* currentLink= list;
	int i= 0;
	while (currentLink){
		if(!strcmp(currentLink->name,name)){
			return currentLink->value;
		}
		currentLink= currentLink->nextLink;
		i++;
	}
	return (char*)-1;
}

/*change the value of the i-th node*/
void changeListValueInLoc(List* list, char* name, char* value, int loc){
	List* currentLink= list;
	char* newValue;
	int i;
	for (i= 0;i < loc; i++){
		currentLink= currentLink->nextLink;
	}
	free(currentLink->value);
	newValue= (char*)malloc(strlen(value)+1);
	strcpy(newValue, value);
	currentLink->value= newValue;
}

/*add name-value pair to list*/
List* addToList(List* list, char* name, char* value){
	char* newValue;
	List* newNode;
	List* currentLink;
	List* prevLink;
	if (!list){ 									 	/*if the list is empty, create a new list*/
		newNode= createNode(name, value, 0);
		list= newNode;
	}
	else {
		currentLink= list;
		while (currentLink){ 							/*look for name in the list*/
			if(!strcmp(currentLink->name,name)){
				break;
			}
			prevLink= currentLink;
			currentLink= currentLink->nextLink;
		}
		if (currentLink){ 								/*a node with the same name was found*/
			newValue= (char*)malloc(strlen(value)+1);
			strcpy(newValue, value);
			free(currentLink->value);
			currentLink->value= newValue; 				/*replace that node's value*/
		}
		else { 											/*no node with the same name was found, add a new node*/
			newNode= createNode(name, value, 0);
			prevLink->nextLink= newNode;
		}
	}
	return list;
}

/*delete name-value pair to list*/
List* deleteFromList(List* list, char* name){
	int i;
	List* currentLink= list;
	List* tmpNode;
	if (!list){
		return NULL;
	}
	if (!strcmp(currentLink->name, name)){ 					/*look for node in the list's head*/
		list= currentLink->nextLink;
		deleteNode(currentLink); 							/*if found, delete and exit*/
		return list;
	}
	while (currentLink){ 									/*look for the node in the list, stop when found*/
		if((currentLink->nextLink) && (!strcmp(currentLink->nextLink->name,name))){
			break;
		}
		currentLink= currentLink->nextLink;
		i++;
	}
	if (!currentLink){ 										/*if not found, do nothing*/
		return list;
	}
	tmpNode= currentLink; 									/*else, delete the node*/
	currentLink= currentLink->nextLink;
	tmpNode->nextLink= currentLink->nextLink;
	deleteNode(currentLink);
	return list;
}

/*clear the list, freeing all memory allocated*/
void freeList(List* list){
	List* currentLink= list;
	List* tempLink;
	while (currentLink){
		tempLink= currentLink->nextLink;
		deleteNode(currentLink);
		currentLink= tempLink;
	}
}

/*print the contants of the list*/
void printList(List* list){
	List* currentLink= list;
	printf("<name,value>:\n");
	while (currentLink){
		printf("\t<%s,%s>\n", currentLink->name, currentLink->value);
		currentLink= currentLink->nextLink;
	}
}