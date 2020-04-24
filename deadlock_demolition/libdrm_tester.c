/**
 * Deadlock Demolition Lab
 * CS 241 - Spring 2019
 */
 
#include "libdrm.h"

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_barrier_t barr;

typedef struct task {
	drm_t * drm1;
	drm_t * drm2;
} task_t;

int routine1(task_t * t) {
	pthread_t tid = pthread_self();
	int res = drm_wait(t->drm1, &tid);
	assert(res == 1);
	pthread_barrier_wait(&barr);
	res = drm_wait(t->drm2, &tid); //maybe deadlock cycle
	if (res == 0)
		res = drm_post(t->drm1, &tid);
	assert(res == 1);
	return res;
}

int routine2(task_t * t) {
	pthread_t tid = pthread_self();
	int res = drm_wait(t->drm2, &tid);
	assert(res == 1);
	pthread_barrier_wait(&barr);
	res = drm_wait(t->drm1, &tid); //maybe deadlock cycle
	if (res == 0)
		res = drm_post(t->drm2, &tid);
	assert(res == 1);
	return res;
}

int main() {
	pthread_barrier_init(&barr, NULL, 2);
    drm_t * drm1 = drm_init();
	drm_t * drm2 = drm_init();
	pthread_t tid1;
	pthread_t tid2;

	task_t * t = malloc(sizeof(task_t));
	t->drm1 = drm1;
	t->drm2 = drm2;

	pthread_create(&tid1, NULL, (void*)routine1, (void*)t);
	pthread_create(&tid2, NULL, (void*)routine2, (void*)t);

	int res1 = -1;
	int res2 = -1;

	pthread_join(tid1, (void**)&res1);
	pthread_join(tid2, (void**)&res2);

	drm_destroy(drm1);
	drm_destroy(drm2);
	pthread_barrier_destroy(&barr);
	puts("SUCCESS");
    return 0;
}
