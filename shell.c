#include "List.h"
#include "cmdLog.h"
#include "pipeHelper.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <linux/limits.h>
#include <sys/wait.h>

List* execString(cmdLog* cLog, List* env, char* incoming);

/*executes a simple command. in case of failure, clears all memory allocated (since this is a forked proccess)*/
void execute(cmdLog* cLog, List* env, cmdLine *pCmdLine){
	execvp(pCmdLine->arguments[0], pCmdLine->arguments);
	perror("illegal command");
	freeCmdLines(pCmdLine);
	clearArray(cLog);
    freeList(env);
	_exit(EXIT_FAILURE);
}

/*counts the number of commandline nodes*/
int countNodes(cmdLine *pCmdLine){
	int ans= 0;
	cmdLine* tmp= pCmdLine;
	while (tmp){
		ans++;
		tmp= tmp->next;
	}
	return ans;
}

/*execute the CD command. does not run as a forked proccess*/
void cmdCD(cmdLine* line, char* incoming){
	if(chdir(line->arguments[1])!=0)
		perror("illegal directory");
}

/*prints the log to the screen*/
void cmdShowLog(cmdLog* cLog){
	printArray(cLog);
}

/*looks for the i-th argument in the log. the command to execute is '!i'*/
List* cmdReadLog(cmdLog* cLog, List* env, cmdLine* line){
	char* prevcmd;
	int value;
	int cond1, cond2;

	if (strlen(line->arguments[0]) >= 2){
		value = atoi(&line->arguments[0][1]); /*parse for the value of i- can go up to LOG_SIZE*/
		cond1= (!value) && (strlen(line->arguments[0]) == 2) && (line->arguments[0][1] == '0'); /*element is zero*/
		cond2= (value >= 0) && (value <= LOG_SIZE-1); /*element is in bounds*/
		if (cond1 && cond2){
			prevcmd = readLog(cLog, value);
			if (!prevcmd){
				printf("illegal arguments after '!'\n");	
			}
			else {
				return execString(cLog, env, prevcmd);
			}
		}
	}
	printf("illegal arguments after '!'\n");
	return env;
}

/*sets a new name-value pair*/
List* cmdAssign(List* env, cmdLine* line){
	if (line->argCount != 3){
		printf("'assign' should have only 2 arguments\n");
	}
	else {
		env= addToList(env, line->arguments[1], line->arguments[2]);
	}
	return env;
}

/*deletes a name-value pair*/
List* cmdUnassign(List* env, cmdLine* line){
	if (line->argCount != 2){
		printf("'unassign' should have only one arguments\n");
	}
	else {
		env= deleteFromList(env, line->arguments[1]);
	}
	return env;
}

/*prints all the name-value pairs*/
void cmdEnv(List* env){
	printList(env);
}

/*replaces all instances of 'name' from the name-value pair with 'value'. does so for every name-value pair in the env*/
cmdLine* cmdEnvReplace(List* env, cmdLine* line){
	int i;
	char* value;
	cmdLine* tempLine= line;
	if (!env){ /*empty env*/
		return line;
	}
	while (tempLine){
		for (i= 0; i < tempLine->argCount; i++){
			if (tempLine->arguments[i][0] == '$'){	/*every word that starts with $ should be replaced with the value attached to that name*/
				value= valueOfName(env, &(tempLine->arguments[i][1]));
				if (value == (char*)-1){	/*if no such word is in the env, stop the renaming, and return the old line*/
					printf("the argument '%s' was not found\n", tempLine->arguments[i]);
					return line;
				}
				if (replaceCmdArg(tempLine, i, value) == 0){
					printf("error while replacing argument '%s'\n", tempLine->arguments[i]);
					return line;
				}
			}
		}
		tempLine= tempLine->next;
	}
	return line;
}

/*basic execute branch. sends the input to the function that takes care of that case*/
/*runs as forked proccess, therefore it clears all memory allocated and exits*/
void coreExec(cmdLog* cLog, List* env, cmdLine* line){
	if (line->inputRedirect){
		close(STDIN_FILENO);
		if (!fopen(line->inputRedirect, "r")){
			perror("input source error");
		}
	}
	if (line->outputRedirect){
		close(STDOUT_FILENO);
		if (!fopen(line->outputRedirect, "w")){
			perror("output source error");
		}
	}
	if (!strcmp(line->arguments[0], "env")){
	    cmdEnv(env);
    }
    else if (!strcmp(line->arguments[0], "log")){
		cmdShowLog(cLog);
    }
    else{
		execute(cLog, env, line);
    }
    freeCmdLines(line);
    clearArray(cLog);
    freeList(env);
    _exit(EXIT_SUCCESS);
}

/*in case of pipes, for multiple commands*/
void pipesDetected(cmdLog* cLog, cmdLine* line, List* env){
	int** pipes;
	int i= 0, nodesCount;
	int* rPipe;
	int* lPipe;
	int* PIDs;
	nodesCount= countNodes(line);
	pipes= createPipes(nodesCount-1);
	PIDs= (int*)malloc(sizeof(int)*nodesCount);
	rPipe= rightPipe(pipes, line);
	if (!(PIDs[i]= fork())){ 					/*fork first pipe*/
		close(STDOUT_FILENO);
		dup(rPipe[1]);
		close(rPipe[1]);
		releasePipes(pipes, nodesCount-1);
		coreExec(cLog, env, line);
	}
	close(rPipe[1]);
	for (i= 0; i < nodesCount-2; i++){ 			/*fork all pipes from 2 to n-1*/
		line= line->next;
		rPipe= rightPipe(pipes, line);
		lPipe= leftPipe(pipes, line);
		if (!(PIDs[i+1]= fork())){
			close(STDIN_FILENO);
			dup(lPipe[0]);
			close(lPipe[0]);
			close(STDOUT_FILENO);
			dup(rPipe[1]);
			close(rPipe[1]);
			releasePipes(pipes, nodesCount-1);
			coreExec(cLog, env, line);
		}
		close(lPipe[0]);
		close(rPipe[1]);
	}
	line= line->next;
	lPipe= leftPipe(pipes, line);
	if (!(PIDs[i+1]= fork())){ 					/*fork the last pipe*/
		close(STDIN_FILENO);
		dup(lPipe[0]);
		close(lPipe[0]);
		releasePipes(pipes, nodesCount-1);
		coreExec(cLog, env, line);
	}
	close(lPipe[0]);
	for (i= 0; i < nodesCount; i++){
		waitpid(PIDs[i], 0, 0);
	}
	free(PIDs);
	releasePipes(pipes, nodesCount-1);
}

/*basic execute branch. sends the input to the function that takes care of that case*/
List* execString(cmdLog* cLog, List* env, char* incoming){
	cmdLine* line;
	List* newEnv= env;
	int pid;
	line = parseCmdLines(incoming);
    if (!strcmp(line->arguments[0], "quit")){
    	freeList(env);
		freeCmdLines(line);
    	return (List*)-1;
    }
    if (line->arguments[0][0] == '!'){
    	newEnv= cmdReadLog(cLog, env, line);
		freeCmdLines(line);
    	return newEnv;
	}
	line= cmdEnvReplace(env, line); 	/*replace every word with $ with the value of that word in the env*/
	pushArray(cLog, incoming);
    if (!strcmp(line->arguments[0], "cd")){
    	cmdCD(line, incoming);
    }
    else if (!strcmp(line->arguments[0], "assign")){
    	newEnv= cmdAssign(env, line);
    }
    else if (!strcmp(line->arguments[0], "unassign")){
    	newEnv= cmdUnassign(env, line);
    }
    else{ 								/*basic command, sends to execute*/
		if (!line->next){ 				/*single command*/
			if (!(pid= fork())){
				coreExec(cLog, env, line);
			}
			if(line->blocking != 0){
				waitpid(pid, 0, 0);
			}
		}
		else { 							/*multiple (piped) commands*/
	    	pipesDetected(cLog, line, env);
		}
	}
    freeCmdLines(line);
    return newEnv;
}

/*loops all input until 'quit' was recieved*/
int main (int argc , char* argv[], char* envp[]){
	char my_cwd[PATH_MAX];
	char buffer[2048];
	cmdLog cLog;
	List* env= 0;
	intLog(&cLog);
	while(1){
	    getcwd(my_cwd, PATH_MAX);
	    printf("%s$ ", my_cwd); 	/*prints current shell location*/
	    fgets(buffer, 2048, stdin);
	    if ((buffer[0] != '\n') && ((env= execString(&cLog, env, buffer)) == (List*)-1)){
	    	break;
	    }
	}
	clearArray(&cLog);
	return 0;
}
