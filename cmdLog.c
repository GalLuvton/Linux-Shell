#include "cmdLog.h"

/*initializes the struct*/
void intLog(cmdLog* cLog){
	cLog->size= 0;
}

/*adds a command to the log*/
void pushArray(cmdLog* cLog, char* cmd){
	char* tmp1;
	char* tmp2;
	int i;
	char* newstr = (char*)malloc(strlen(cmd)+1); 	/*allocate new area for the command, copy the old one into it*/
	memcpy(newstr,cmd,strlen(cmd)+1);
	if(cLog->size<LOG_SIZE){ 						/*if log isen't full yet*/
		cLog->str[cLog->size]=newstr;
		cLog->size = (cLog->size)+1;
	}
	else{ 											/*if the log is full*/
		tmp1 = cLog->str[(cLog->size)-1];
		for(i= (cLog->size)-2;i>=0;i--){ 			/*move all log commands one spot to the right*/
			tmp2 = cLog->str[i];
			cLog->str[i]=tmp1;
			tmp1 = tmp2;
		}
		free(tmp1); 								/*remove the oldest command*/
		cLog->str[LOG_SIZE-1] = newstr; 			/*add the new command*/
	}
}

/*prints the log's contants*/
void printArray(cmdLog* cLog){
	int i;
	for(i=0;i<cLog->size;i++){
		printf("%s",cLog->str[i]);
	}
}

/*clears the log, freeing all memory allocated*/
void clearArray(cmdLog* cLog){
	int i;
	for(i=0;i<cLog->size;i++){
		free(cLog->str[i]);
	}
}

/*returns the i-th command from the log*/
char* readLog(cmdLog* cLog,int loc){
	if (cLog->size <= loc){
		return 0;
	}
	return cLog->str[loc];
}