/**
 * Critical Concurrency Lab
 * CS 241 - Spring 2019
 */
 
#include "queue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

/**
 * This queue is implemented with a linked list of queue_nodes.
 */
typedef struct queue_node {
    void *data;
    struct queue_node *next;
} queue_node;

struct queue {
    /* queue_node pointers to the head and tail of the queue */
    queue_node *head, *tail;

    /* The number of elements in the queue */
    ssize_t size;

    /**
     * The maximum number of elements the queue can hold.
     * max_size is non-positive if the queue does not have a max size.
     */
    ssize_t max_size;

    /* Mutex and Condition Variable for thread-safety */
    pthread_cond_t cv;
    pthread_mutex_t m;
};

queue *queue_create(ssize_t max_size) {
    /* Your code here */
	queue * q = malloc(sizeof(queue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	q->max_size = max_size;
	pthread_mutex_init(&(q->m), NULL);
	pthread_cond_init(&(q->cv), NULL);
    return q;
}

void queue_destroy(queue *this) {
    /* Your code here */
	for (ssize_t i=0; i<this->size; i++) {
		queue_pull(this);
	}
	pthread_cond_destroy(&(this->cv));
	pthread_mutex_destroy(&(this->m));
	free(this);
}

void queue_push(queue *this, void *data) {
    /* Your code here */
	pthread_mutex_lock(&(this->m));
	if (this->max_size >= 0) {
		while (this->size == this->max_size) {
			//perror("PUSH: blocking max size");
			pthread_cond_wait(&(this->cv), &(this->m));
		}
	}

	//push to tail and update tail & size
	queue_node * node = malloc(sizeof(queue_node));
	node->data = data;
	if (this->tail != NULL) {
		(this->tail)->next = node;
		//printf("TAIL: %p\n", this->tail);
	}
	node->next = NULL;
	this->tail = node;
	if (this->size == 0)
		this->head = node;
	this->size++;

	pthread_cond_signal(&(this->cv));
	pthread_mutex_unlock(&(this->m));
	//pthread_cond_signal(&(this->cv));
}

void *queue_pull(queue *this) {
    /* Your code here */
	pthread_mutex_lock(&(this->m));
	while (this->size == 0) {
		//perror("PULL: blocking empty");
		pthread_cond_wait(&(this->cv), &(this->m));
	}

	//pull from head and update head & size
	queue_node * head = this->head;
	void * data = head->data;
	this->head = head->next;
	if (this->size == 1)
		this->tail = NULL;
	//printf("FREEING HEAD: %p\n", head);
	free(head);
	head = NULL;
	this->size--;

	//usleep(1);
	pthread_mutex_unlock(&(this->m));
    return data;
}
