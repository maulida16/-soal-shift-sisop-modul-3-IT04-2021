#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int pid, status;
int pipe1[2];
int pipe2[2];

int main() {
if (pipe(pipe1) == -1){printf("Andri"); exit(1);}
if (pipe(pipe2) == -1){printf("Andri lagi"); exit(1);}

	if ((fork()) == 0) {


		close(pipe1[0]);
		dup2(pipe1[1], STDOUT_FILENO);
	
		char *argv1[] = {"ps", "-aux", NULL};
		execv("/bin/ps", argv1);
	}

	else {

		while(wait(&status) > 0);
		if ((fork()) == 0) {
		close(pipe1[1]);
		dup2(pipe1[0], STDIN_FILENO);
		close(pipe2[0]);
		dup2(pipe2[1], STDOUT_FILENO);
		// exec
		char *argv1[] = {"sort", "-nrk", "3,3", NULL};
		execv("/bin/sort", argv1);
		} 
		
		else {
		while(wait(&status) > 0);

			// input from pipe1
			close(pipe2[1]);
			dup2(pipe2[0], STDIN_FILENO);

			char *argv1[] = {"head", "-5", NULL};
			execv("/usr/bin/head", argv1);

		}
	}
}
