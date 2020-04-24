/**
 * Utilities Unleashed Lab
 * CS 241 - Spring 2019
 */

#include "format.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc < 2)
		print_env_usage();
	pid_t child = fork();
	if (child < 0) //fork failed
		print_fork_failed();
	else if (child > 0) { //parent
		int status;
		waitpid(child, &status, 0);
		if (WEXITSTATUS(status) != EXIT_SUCCESS)
			exit(1);
	}
	else { //child
		int i;
		int found = 0;
		for (i = 1; i < argc; i++) {
			if (strcmp(argv[i], "--")==0) {
				found = 1;
				break;
			}
			char * str = strdup(argv[i]);
			char * key = strtok(str, "=");
			if (key == NULL)
				print_env_usage();
			char * val = strtok(NULL, "=");
			if (val == NULL)
				val = "";
			else if (*val == '%') //reference
				val = getenv(val+1);
			if (val == NULL)
				print_environment_change_failed();
			int set = setenv(key, val, 1);
			char * curr_val = getenv(key);
			if (curr_val == NULL)
				print_environment_change_failed();
			if (set == -1)
				print_environment_change_failed();
		}
		if (i == argc || (i==argc-1 && found==1))
			print_env_usage();

		execvp(argv[i+1], argv+i+1);
		print_exec_failed();
	}
    return 0;
}
