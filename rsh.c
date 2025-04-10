#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// TODO
	for (int i = 0; i < N; i++) {
		if (!strcmp(cmd, *(allowed + i))) {
			return 1;
		}
	}
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	// comment test
	
	return 0;
}

int main() {

    // TODO
    // Add variables as needed
    
    char* argv[20]; 
    int status;
    pid_t pid;
    posix_spawnattr_t attr;
    posix_spawnattr_init(&attr);
    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSCHEDULER);

    char line[256];
    char lineCpy[256];
    char* argTok = NULL;
    int numArgs;

    while (1) {

	fprintf(stderr,"rsh>");	

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';

	strcpy(lineCpy, line);

	argTok = strtok(lineCpy, " ");

	*argv = argTok;

	numArgs = 0;

	for (int i = 1; argTok != NULL; i++) { // Add arguments to argv array (including NULL)
		argTok = strtok(NULL, " ");
		*(argv + i) = argTok;
		numArgs = i - 1;
	}	
	
	if (isAllowed(lineCpy)) {
		if (strcmp(lineCpy, "cd") && strcmp(lineCpy, "help") && strcmp(lineCpy, "exit")) {
			if (posix_spawnp(&pid, lineCpy, NULL, &attr, argv, environ) != 0) {
				perror("spawn failed\n");
				exit(EXIT_FAILURE);
			}
        		if (waitpid(pid, &status, 0) == -1) {
				perror("waitpid failed\n");
				exit(EXIT_FAILURE);
			}	
		} else if (!strcmp(lineCpy, "cd")) {
			if (numArgs > 1) {
				printf("-rsh: cd: too many arguments\n");
			} else {
				chdir(argv[1]);
			}
		} else if (!strcmp(lineCpy, "help")) {
			printf("The allowed commands are:\n");
			printf("1: cp\n");
			printf("2: touch\n");
			printf("3: mkdir\n");
			printf("4: ls\n");
			printf("5: pwd\n");
			printf("6: cat\n");
			printf("7: grep\n");
			printf("8: chmod\n");
			printf("9: diff\n");
			printf("10: cd\n");
			printf("11: exit\n");
			printf("12: help\n");
		} else {
			posix_spawnattr_destroy(&attr);
			break;
		}
	} else {
		printf("NOT ALLOWED!\n");
	}

	/*
	while (argTok != NULL) {
		printf("argTok is: %s\n", argTok);
                printf("lineCpy is: %s\n", lineCpy);
		argTok = strtok(NULL, " ");
	}
	*/

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

    }
    return 0;
}
