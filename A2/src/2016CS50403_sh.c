// for input and output
#include <stdio.h>
// for parsing arguments and comparing
#include <string.h>
// for systerm calls
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h> 

char mainInput[2000];

struct command {
	char* input;
	char* cmdArgsArr[10];
	int argCount;
	char outStreamBuff[80];
	char inStreamBuff[80];
	int outChanged;
	int inChanged;
};

struct command parallelCommands[10];
int parallelCmdCount;

mode_t permissionMask = 0755;

int parseCmd() {
	char* mainInp = mainInput;
	int cmdIndex = 0;

	fgets(mainInp, sizeof(mainInput), stdin);

	if (*mainInp == '\n') mainloop();

	parallelCommands[cmdIndex].input = strtok(mainInp, "|\n");
	cmdIndex++;

	while(1) {
		parallelCommands[cmdIndex].input = strtok(NULL, " \n");
		if (parallelCommands[cmdIndex].input == NULL) break;
		parallelCmdCount++;
		cmdIndex++;
	}
	parallelCmdCount = cmdIndex;
	return 0;
}

int parseSectionCmd(int commandIndex) {
	struct command* currentCmd = &parallelCommands[commandIndex];

	char* inp = currentCmd->input;
	char** cmdArgs = currentCmd->cmdArgsArr;
	char* outStream = currentCmd->outStreamBuff;
	char* inStream = currentCmd->inStreamBuff;

	currentCmd->inChanged = 0;
	currentCmd->outChanged = 0;

	if (*inp == ' ') inp++;

	int argCountHere = 0;
	char* cutPoint = strtok(inp, " ");
	cmdArgs[argCountHere] = cutPoint;
	argCountHere++;

	while (1) {
		cutPoint = strtok(NULL, " ");
		if (cutPoint == NULL) break;

		char cmd0 = *cutPoint;
		char cmd1 = *(cutPoint+1);
		if (cmd0 == '<') {
			if (cmd1 == '\0') {
				strcpy(inStream, strtok(NULL, " "));
			} else {
				strcpy(inStream, cutPoint+1);
			}
			currentCmd->inChanged = 1;
		} else if (cmd0 == '>') {
			if (cmd1 == '\0') {
				strcpy(outStream , strtok(NULL, " "));
			}
			else {
				strcpy(outStream, cutPoint+1);
			}
			currentCmd->outChanged = 1;
		}
		if ((cmd0 == '<') || (cmd0 == '>')) {
			cutPoint = NULL;
			argCountHere--;
		} else {
			cmdArgs[argCountHere] = cutPoint;
		}
		argCountHere++;
	}
	currentCmd->argCount = argCountHere;
	return 0;
}

int cd(const char* dir) {
	int errorCode = chdir(dir);
	if (errorCode != 0) {
		printf("Could not change directory to %s\nError Code: %d\n", dir, errorCode);
	} else {
		char pwd[100];
		printf("Changed directory to %s\n", getcwd(pwd, 100));
	}
	return 0;
}

int pwd() {
	char pwdStore[100];
	printf("Present work directory: %s\n", getcwd(pwdStore, 100));
	return 0;
}

int mkdirec(const char* dir) {
	int errorCode = mkdir(dir, permissionMask);
	if (errorCode != 0) {
		printf("Failed to create directory: %s\nError Code: %d\n", dir, errorCode);
	} else {
		printf("Created directory: %s\n", dir);
	}
	return errorCode;
}

int rmdirec(const char* dir) {
	int errorCode = rmdir(dir);
	if (errorCode != 0) {
		printf("Failed to remove directory: %s\nError Code: %d\n", dir, errorCode);
	} else {
		printf("Removed directory: %s\n", dir);
	}
	return errorCode;
}

int exiter() {
	printf("Exit commmand called\nExiting.....\n");
	exit(0);
}

int execute(char** argv) {
	int errorCode = execvp(argv[0], argv);
	printf("Unknown commnad\n");
	return errorCode;
}

int child_status_combined;
int childsNotReturned;

int pipes[9][2];

int makePipe(int pipeIndex) {
	int errorCode = pipe(pipes[pipeIndex]);
	if (errorCode < 0) {
		printf("System failed to pipe%n");
		mainloop();
	}
	return errorCode;
}

int execCmd() {
	child_status_combined = 0;
	childsNotReturned = parallelCmdCount;

	for (int i=0; i<parallelCmdCount; i++) {
		parseSectionCmd(i);
		char** argv = parallelCommands[i].cmdArgsArr;

		// // sample code
		// // open input and output files
		// int in = open("scores", O_RDONLY);
		// int out = open("out", O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);

		// // replace standard input with input file
		// dup2(in, 0);

		// // replace standard output with output file
		// dup2(out, 1);

		// // close unused file descriptors
		// close(in);
		// close(out);

		// connnect pipes
		if (parallelCmdCount > 1) {
			if (i == 0) {
				makePipe(i);
				dup2(pipes[i][1], 1);
				close(pipes[i][1]);
			} else if (i == parallelCmdCount-1) {
				dup2(pipes[i-1][0], 0);
				close(pipes[i-1][0]);
			} else {
				makePipe(i);
				dup2(pipes[i][1], 1);
				close(pipes[i][1]);
				dup2(pipes[i-1][0], 0);
				close(pipes[i-1][0]);
			}
		}

		int errorCode = 0;
		if (strcmp(argv[0], "exit") == 0) exit(0);
		else if (strcmp(argv[0], "cd") == 0) {
			errorCode = cd(argv[1]) ; 
			childsNotReturned--;
		}
		else if (strcmp(argv[0], "pwd") == 0) {
			errorCode = pwd(); 
			childsNotReturned--;
		}
		else if (strcmp(argv[0], "mkdir") == 0) {
			errorCode = mkdirec(argv[1]); 
			childsNotReturned--;
		}
		else if (strcmp(argv[0], "rmdir") == 0) {
			errorCode = rmdirec(argv[1]); 
			childsNotReturned--;
		} else {
			int ret_pid = fork();
			if(ret_pid == 0) {
				/* This is done by the child process. */

				// connect input output
				if (parallelCommands[i].inChanged) {
					int in = open(parallelCommands[i].inStreamBuff, O_RDONLY);
					dup2(in, 0);
					close(in);
				}

				if (parallelCommands[i].outChanged) {
					int out = open(parallelCommands[i].outStreamBuff, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					dup2(out, 1);
					close(out);
				}
				
				printf("%d: %s\n", ret_pid, argv[0]);
				exit(execute(argv));
			}
		}	

		child_status_combined = child_status_combined || errorCode;
	}
	/* This is run by the parent.  Wait for the child
	to terminate. */
	while (childsNotReturned>0) {
		int childStatus;
		pid_t childId = wait(&childStatus);
		printf("%d\n", childId);
		child_status_combined = child_status_combined || 1;

		if (childId == -1) return -1;
		else childsNotReturned--;
	}
	return 0;
}

int mainloop() {
	while(1) {
		printf("shell> ");
		parseCmd();
		execCmd();
	}
	return 0;
}

int main() {
	mainloop();
	return 0;
}