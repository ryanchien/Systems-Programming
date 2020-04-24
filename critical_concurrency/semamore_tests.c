/**
 * Critical Concurrency Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "semamore.h"

void * routine(void * s) {
	for (int i=0; i<5; i++) {
		semm_wait(s);
		sleep(1);
	}
	return NULL;
}

int main(int argc, char **argv) {
    //printf("Please write tests in semamore_tests.c\n");
	Semamore * s = malloc(sizeof(Semamore));
	semm_init(s, 0, 10);
	pthread_t * arr = malloc(sizeof(pthread_t) * 2);

	for (int i=0; i<2; i++) {
		pthread_create(&arr[i], NULL, routine, s);
	}

	for (int i=0; i<10; i++) {
		semm_post(s);
	}

	for (int i=0; i<2; i++) {
		pthread_join(arr[i], NULL);
	}
	puts("SUCCESS");
	free(arr);
	semm_destroy(s);
    return 0;
}
