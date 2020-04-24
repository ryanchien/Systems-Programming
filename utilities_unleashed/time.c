/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */
 
#include "format.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
	if (argc < 2)
		print_time_usage();
	pid_t child = fork();
	if (child < 0) //fork failed
		print_fork_failed();
	else if (child > 0) { //parent
		int status;
		struct timespec start, end;
		clock_gettime(CLOCK_MONOTONIC, &start);
		waitpid(child, &status, 0);
		if(WEXITSTATUS(status) != EXIT_SUCCESS) //child failed
			exit(1);
		clock_gettime(CLOCK_MONOTONIC, &end);
		double time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)*1e-9;
		display_results(argv, time);
	}
	else { //child
		execvp(argv[1], argv+1);
		print_exec_failed(); //only executes if exec fails
	}
    return 0;
}
