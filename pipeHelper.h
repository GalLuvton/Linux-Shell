#include "LineParser.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*
This function receives the number of required pipes and returns an array of pipes.
*/
int **createPipes(int nPipes);

/*
This function receives an array of pipes and an integer indicating the size of the array. The function releases all memory dedicated to the pipes.
*/
void releasePipes(int **pipes, int nPipes);

/*
This function receives an array of pipes and a pointer to a cmdLine structure. It returns the pipe which feeds the process associated with the command. That is, the pipe that appears to the left of the process name in the command line.
For example, the left pipe of process tee in pipeline "cat | tee | more" is the first pipe. If the command does not have a left pipe (as with cat in our example), the function returns NULL.
*/
int *leftPipe(int **pipes, cmdLine *pCmdLine);

/*
This function receives an array of pipes and a pointer to a cmdLine structure. It returns the pipe which is the sink of the associated command. That is, the pipe that appears to the right of the process name in the command line.
For example, the right pipe of process tee in pipeline "cat | tee | more" is the second pipe. If the command does not have a right pipe (as with more in our example), the function returns NULL.
*/
int *rightPipe(int **pipes, cmdLine *pCmdLine);