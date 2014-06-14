#include "pipeHelper.h"

/*This function receives the number of required pipes and returns an array of pipes*/
int** createPipes(int nPipes){
	int i, err= 0;
	int** pipes;
	pipes= (int**)malloc(sizeof(int*)*nPipes);
	for (i= 0; ((!err) && (i < nPipes)); i++){
		pipes[i]= (int*)malloc(sizeof(int)*2);
		if (pipe(pipes[i]) == -1) {
		   err= 1;
		}
	}
	return pipes;
}

/*This function receives an array of pipes and an integer indicating the size of the array.
The function releases all memory dedicated to the pipes*/
void releasePipes(int **pipes, int nPipes){
	int i;
	for (i= 0; i < nPipes; i++){
		free(pipes[i]);
	}
	free(pipes);
}

/*This function receives an array of pipes and a pointer to a cmdLine structure.
It returns the pipe which feeds the process associated with the command.
That is, the pipe that appears to the left of the process name in the command line.
For example, the left pipe of process tee in pipeline "cat | tee | more" is the first pipe.
If the command does not have a left pipe (as with cat in our example), the function returns NULL*/
int* leftPipe(int **pipes, cmdLine *pCmdLine){
	cmdLine* tmp;
	int ans;
	int last;
	ans= pCmdLine->idx-1;
	tmp= pCmdLine;
	while (tmp->next){
		tmp= tmp->next;
	}
	last= tmp->idx;
	if ((ans >= 0) && (ans < last)){
		return pipes[ans];
	}
	return NULL;
}

/*This function receives an array of pipes and a pointer to a cmdLine structure.
It returns the pipe which is the sink of the associated command.
That is, the pipe that appears to the right of the process name in the command line.
For example, the right pipe of process tee in pipeline "cat | tee | more" is the second pipe.
If the command does not have a right pipe (as with more in our example), the function returns NULL*/
int* rightPipe(int **pipes, cmdLine *pCmdLine){
	cmdLine* tmp;
	int ans;
	int last;
	ans= pCmdLine->idx;
	tmp= pCmdLine;
	while (tmp->next){
		tmp= tmp->next;
	}
	last= tmp->idx;
	if ((ans >= 0) && (ans < last)){
		return pipes[ans];
	}
	return NULL;
}
