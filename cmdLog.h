#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LOG_SIZE 3

/*Log array struct*/
typedef struct cmdLog
{
    char * str[LOG_SIZE]; 	/*logged commands*/
    int size; 	/*how many commands logged*/
} cmdLog;


/*initializes the struct*/
void intLog(cmdLog* cLog);

/*adds a command to the log*/
void pushArray(cmdLog* cLog, char* cmd);

/*prints the log's contants*/
void printArray(cmdLog* cLog);

/*clears the log, freeing all memory allocated*/
void clearArray(cmdLog* cLog);

/*returns the i-th command from the log*/
char* readLog(cmdLog* cLog,int loc);
