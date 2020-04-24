/**
 * Critical Concurrency Lab
 * CS 241 - Spring 2019
 */
 
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "queue.h"

typedef struct task {
	queue * q;
	int * data;
} task_t;

void * routine (task_t * t) {
	queue_push(t->q, t->data);
	printf("[%d]: %d\n", *(t->data), *(int*)queue_pull(t->q));
	return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s test_number\n", argv[0]);
        exit(1);
    }

	size_t n = (size_t)atoi(argv[1]);
	queue * q = queue_create(n);
	pthread_t * threads = malloc(sizeof(pthread_t) * n);

	task_t * t = malloc(sizeof(task_t) * n);
	int * data = malloc(sizeof(int) * n);

	for (size_t i=0; i<n; i++) {
		t[i].q = q;
		data[i] = i;
		t[i].data = data+i;
		pthread_create(threads+i, NULL, (void*)routine, (void*)&t[i]);
	}

	for (size_t i=0; i<n; i++) {
		pthread_join(threads[i], NULL);
	}
/*
	for (size_t i=0; i<n; i++) {
		//queue_pull(q);
		printf("[%lu]: %d\n", i, *(int*)queue_pull(q));
	}
*/

	queue_destroy(q);
	free(t);
	free(data);
	free(threads);
	puts("test 1 pass");

	queue * q1 = queue_create(-1);
	int * data1 = malloc(sizeof(int)*11);
	for (int i=0; i<11; i++) {
		data1[i] = i;
		queue_push(q1, data1+i);
	}
	
	//printf("[0]: %d\n", *(int*)queue_pull(q));

	for (int i=0; i<11; i++) {
		printf("[%d]: %d\n", i, *(int*)queue_pull(q));
	}
	puts("test 2 pass");
	queue_destroy(q1);
	free(data1);

	puts("SUCCESS");
    return 0;
}
