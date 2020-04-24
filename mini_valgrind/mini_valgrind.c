/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2019
 */
 
#include "mini_valgrind.h"
#include <stdio.h>
#include <string.h>

//GLOBALS
meta_data * head = NULL;
size_t total_memory_requested = 0;
size_t total_memory_freed = 0;
size_t invalid_addresses = 0;

void *mini_malloc(size_t request_size, const char *filename,
                  void *instruction) {
	if (request_size == 0)
		return NULL;

    //allocate space for meta_data + memory
	void * space = malloc(sizeof(meta_data) + request_size);
	if (space == NULL)
    	return NULL;
	
	//set ptrs
	meta_data * ptr1 = (meta_data*) space;
	void * ptr2 = space + sizeof(meta_data);

	//init meta_data
	ptr1->request_size = request_size;
	ptr1->filename = filename;
	ptr1->instruction = instruction;
	ptr1->next = head;
	head = ptr1;

	total_memory_requested += request_size;
	return ptr2;
}

void *mini_calloc(size_t num_elements, size_t element_size,
                  const char *filename, void *instruction) {
	if (num_elements==0 || element_size==0)
		return NULL;
	size_t request_size = num_elements * element_size;

	//allocate mem
	void * ptr2 = mini_malloc(request_size, filename, instruction);

	//init payload
	ptr2 = memset(ptr2, 0, request_size);
    return ptr2;
}

void *mini_realloc(void *payload, size_t request_size, const char *filename,
                   void *instruction) {
	if (payload == NULL)
		return mini_malloc(request_size, filename, instruction);
	if (request_size == 0) {
		mini_free(payload);
		return NULL;
	}

	meta_data * ptr1 = payload - sizeof(meta_data);
	meta_data * curr = head;
	meta_data * prev = NULL;
	meta_data * next = NULL;

	//find node and realloc
	while (curr != NULL) {
		//node found
		if (curr == ptr1) {
			if (curr->request_size < request_size)
				total_memory_requested += (request_size - curr->request_size);
			else if (curr->request_size > request_size)
				total_memory_freed += (curr->request_size - request_size);
			//size is the same -> just update metadata values
			else {
				curr->filename = filename;
				curr->instruction = instruction;
				return curr;
			}
			next = curr->next;
			//realloc space for metadata+payload
			void * space = realloc(curr, sizeof(meta_data) + request_size);
			if (space == NULL)
				return NULL;
			meta_data * ptr1 = (meta_data*) space;
			void * ptr2 = space + sizeof(meta_data);
			//init metadata
			ptr1->request_size = request_size;
			ptr1->filename = filename;
			ptr1->instruction = instruction;
			ptr1->next = next;
			return ptr2;
		}
		prev = curr;
		curr = curr->next;
	}
	//node not found
	invalid_addresses++;
    return NULL;
}

void mini_free(void *payload) {
	if (payload == NULL)
		return;

	meta_data * ptr1 = payload - sizeof(meta_data);
	meta_data * curr = head;
	meta_data * prev = NULL;

	//find node and remove
	while (curr != NULL) {
		//node found
		if (curr == ptr1) {
			//node is head
			if (prev == NULL)
				head = curr->next;
			else
				prev->next = curr->next;
			total_memory_freed += curr->request_size;
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}

	//node not found = bad free
	invalid_addresses++;
}
