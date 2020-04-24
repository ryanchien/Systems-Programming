/**
 * Teaching Threads Lab
 * CS 241 - Spring 2019
 */
 
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "reduce.h"
#include "reducers.h"

/* You might need a struct for each task ... */
struct task {
	int * list;
	//size_t len;
	size_t start;
	size_t end;
	reducer red;
	int base;
};
typedef struct task task_t;
/* You should create a start routine for your threads. */
int * routine(task_t * t) {
/*
	puts("routine list: ");
	for (size_t i = t->start; i < t->end; i++) {
		printf("[%d]", t->list[i]);
	}
 	puts("\n");
*/
	for (size_t i=t->start; i<t->end; i++) {
		t->base = t->red(t->base, t->list[i]);
	}
	return &(t->base);
}

int par_reduce(int *list, size_t list_len, reducer reduce_func, int base_case,
               size_t num_threads) {
	int result = base_case;
	double elems_per_thread = round((double)list_len / (double)num_threads);
	size_t ept = elems_per_thread==0 ? 1:(size_t)elems_per_thread;

	int * list_heap = malloc(sizeof(int) * list_len);
	for (size_t i = 0; i < list_len; i++) {
		list_heap[i] = list[i];
	}
/* printing out list for debugging
	puts("list: ");
	for (size_t i = 0; i < list_len; i++) {
		printf("[%d]", list[i]);
	}
	puts("\n");
*/

	
	task_t * t = malloc(sizeof(task_t) * num_threads);
	pthread_t * id = malloc(sizeof(pthread_t) * num_threads);
	//task_t * res;

	size_t start = 0;
	size_t end = 0;
	size_t i = 0;
	for (; i < num_threads; i++) {
		if (i == list_len)
			break;
		t[i].list = list_heap;
		//make sure last thread includes elems until end of list
		if (i == num_threads-1)
			end = list_len;
		else
			end = start + ept;
		//printf("thread [%lu] num elems: %lu\n", i, end-start);
		
		t[i].red = reduce_func;
		t[i].base = base_case;
		t[i].start = start;
		t[i].end = end;
/*
		printf("t start: %lu end: %lu\n", t[i].start, t[i].end);
		puts("t list: ");
		for (size_t j = t[i].start; j < t[i].end; j++) {
			printf("[%d]", t[i].list[j]);
		}
 		puts("\n");
*/
		pthread_create(id+i, NULL, (void*)routine, (void*)&t[i]);
		start = end;
	}

	int * result_list = malloc(sizeof(int) * i);
	int * res;
	for (size_t j = 0; j < i; j++) {
		pthread_join(id[j], (void**)&res);
		/*printf("thread list %lu: ", i);
		for (size_t i = 0; i < res->len; i++) {
			printf("[%d]", res->list[i]);
		}
		puts("\n");
		printf("thread res: %d\n", res->base);
		*/
		result_list[j] = *res;
		//result = reduce_func(result, res->base);
	}

	result = reduce(result_list, i, reduce_func, base_case);

	//result = t->base;
	free(t);
	free(id);
	free(list_heap);
	free(result_list);
	//printf("res: %d\n", result);
    return result;
}
