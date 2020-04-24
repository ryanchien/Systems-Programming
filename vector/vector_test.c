/**
 * Vector Lab
 * CS 241 - Spring 2019
 */
 
#include "vector.h"
#include "callbacks.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    // Write your test cases here
	vector * v = vector_create(int_copy_constructor, int_destructor, int_default_constructor);
	assert(vector_empty(v));
	size_t cap = vector_capacity(v);

	int a = 5;
	int b = 30;
	vector_push_back(v, &a);
	vector_pop_back(v);
	assert(vector_empty(v));
	assert(vector_size(v) == 0);
	
	vector_push_back(v, &a);
	vector_push_back(v, &b);
	assert(!vector_empty(v));
	assert(vector_size(v) == 2);
	assert(*(int*)(vector_get(v, 0)) == 5);
	assert(*(int*)(vector_get(v, 1)) == 30);
	assert(**(int**)(vector_front(v)) == 5);
	assert(**(int**)(vector_begin(v)) == 5);
	assert(**(int**)(vector_back(v)) == 30);


	int c = 10;
	int d = 8;
	int e = 25;
	vector_insert(v, 1, (void*)&c);
	vector_insert(v, 1, (void*)&d);
	vector_insert(v, 3, (void*)&e);
	assert(vector_size(v) == 5);
	assert(**(int**)(vector_at(v, 0)) == 5);
	assert(**(int**)(vector_at(v, 1)) == 8);
	assert(**(int**)(vector_at(v, 2)) == 10);
	assert(**(int**)(vector_at(v, 3)) == 25);
	assert(**(int**)(vector_at(v, 4)) == 30);

	int f = 7;
	vector_set(v, 1, (void*)&f);
	assert(*(int*)(vector_get(v, 1)) == 7);

	vector_erase(v, 1);
	assert(vector_size(v) == 4);
	assert(*(int*)(vector_get(v, 0)) == 5);
	assert(*(int*)(vector_get(v, 1)) == 10);
	assert(*(int*)(vector_get(v, 2)) == 25);
	assert(*(int*)(vector_get(v, 3)) == 30);

	vector_pop_back(v);
	assert(vector_size(v) == 3);
	assert(*(int*)(vector_get(v, 0)) == 5);
	assert(*(int*)(vector_get(v, 1)) == 10);
	assert(*(int*)(vector_get(v, 2)) == 25);
	
	vector_push_back(v, &b);
	assert(vector_size(v) == 4);
	vector_resize(v, 3);
	assert(vector_size(v) == 3);
	assert(*(int*)(vector_get(v, 0)) == 5);
	assert(*(int*)(vector_get(v, 1)) == 10);
	assert(*(int*)(vector_get(v, 2)) == 25);

	int g = 0;
	int h = 15;
	vector_insert(v, 0, (void*)&g);
	vector_insert(v, 3, (void*)&h);
	assert(vector_size(v) == 5);
	assert(*(int*)(vector_get(v, 0)) == 0);
	assert(*(int*)(vector_get(v, 1)) == 5);
	assert(*(int*)(vector_get(v, 2)) == 10);
	assert(*(int*)(vector_get(v, 3)) == 15);
	assert(*(int*)(vector_get(v, 4)) == 25);

	vector_erase(v, 0);
	vector_erase(v, 3);
	assert(vector_size(v) == 3);
	assert(*(int*)(vector_get(v, 0)) == 5);
	assert(*(int*)(vector_get(v, 1)) == 10);
	assert(*(int*)(vector_get(v, 2)) == 15);

	vector_reserve(v, 10);
	//printf("cap: %zu\n", vector_capacity(v));
	assert(vector_capacity(v) == cap*2);

	vector_clear(v);
	assert(vector_empty(v));

	vector_destroy(v);

	

	puts("SUCCESS");
    return 0;
}
