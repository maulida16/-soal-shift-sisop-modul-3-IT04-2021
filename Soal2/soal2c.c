#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int pid, status;
int pipe1[2];
int pipe2[2];

int main() {
	printf("Halo lur!\n");
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
		close(pipe2[0]);
		// printf("---masuk child 2\n");
		dup2(pipe1[0], STDIN_FILENO);
		dup2(pipe2[1], STDOUT_FILENO);
		
		char *argv1[] = {"sort", "-nrk", "3,3", NULL};
		execv("/usr/bin/sort", argv1);

		} 
		
		else {

			// // input from pipe1
			close(pipe1[0]);
			close(pipe1[1]);
			close(pipe2[1]);
			while(wait(&status) > 0);
			// printf("---masuk parent");
			dup2(pipe2[0], STDIN_FILENO);
			close(pipe2[0]);
			
			char *argv1[] = {"head", "-5", NULL};
			execv("/usr/bin/head", argv1);
			
		}
	}
}
