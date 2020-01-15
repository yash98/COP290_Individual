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

	parallelCommands[cmdIndex].input = strtok(mainInp, "|\n");
	cmdIndex++;
	parallelCmdCount = 1;

	while(1) {
		parallelCommands[cmdIndex].input = strtok(NULL, " \n");
		if (parallelCommands[cmdIndex].input == NULL) break;
		parallelCmdCount++;
	}
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

	cmdArgs[currentCmd->argCount] = strtok(inp, " ");
	currentCmd->argCount = 1;

	while (1) {
		cmdArgs[currentCmd->argCount] = strtok(NULL, " ");
		if (cmdArgs[currentCmd->argCount] == NULL) break;
		char cmd0 = *cmdArgs[currentCmd->argCount];
		char cmd1 = *(cmdArgs[currentCmd->argCount]+1);
		if ((cmd0 == '<') || (cmd0 == '>')) {
			if (cmd1 == '\0') {
				if (cmd0 == '<') {
					strcpy(inStream, strtok(NULL, " "));
					currentCmd->inChanged = 1;
				} else {
					strcpy(outStream , strtok(NULL, " "));
					currentCmd->outChanged = 1;
				}
				continue;
			} else {
				cmdArgs[currentCmd->argCount]++;
			}
		}
		currentCmd->argCount++;
	}
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

int execCmd() {
	child_status_combined = 0;
	childsNotReturned = parallelCmdCount;

	for (int i=0; i<parallelCmdCount; i++) {
		parseSectionCmd(i);
		char** argv = parallelCommands[i].cmdArgsArr;
		
		if (strcmp(argv[0], "exit") == 0) exit(0);
		
		int ret_pid = fork();
		if(ret_pid == 0) {
			/* This is done by the child process. */
			if (strcmp(argv[0], "cd") == 0) exit(cd(argv[1]));
			else if (strcmp(argv[0], "pwd") == 0) exit(pwd());
			else if (strcmp(argv[0], "mkdir") == 0) exit(mkdirec(argv[1]));
			else if (strcmp(argv[0], "rmdir") == 0) exit(rmdirec(argv[1]));
			else exit(execute(argv));
		}	
	}
	/* This is run by the parent.  Wait for the child
	to terminate. */
	while (childsNotReturned>0) {
		int childStatus;
		pid_t childId = wait(&childStatus);
		if (childStatus > 0) child_status_combined = 1;

		if (childId == -1) return -1;
		else childsNotReturned--;
	}
	return 0;
}

int main() {
	while(1) {
		printf("shell> ");
		parseCmd();
		execCmd();
	}
}

// This shell cant do multiple redirections or complex pipes like
// 	cat < a.txt < b.txt
// or 
// 	cmd1 | cmd2 | cmd 
// or even 
// 	cmd1 < a.txt | cmd2 > b.txt
// but it can do 
// 	cmd1 < a.txt > b.txt