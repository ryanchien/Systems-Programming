/**
 * Malloc Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <math.h>

#define SPLIT_THRESHOLD 1024*2*2*2*2*2*2*2

//implemented using explicit free list
typedef struct meta_data {
	size_t size; //denotes size of data block
	char free; //0 for used, 1 for free
	struct meta_data * prev;
	struct meta_data * next;
	struct meta_data * prev_free;
	struct meta_data * next_free;
} md_t;

//static md_t * used_head = NULL;
static md_t * used_tail = NULL;
static md_t * free_head = NULL;

void remove_free(md_t * md) {
	if (md->prev_free != NULL)
		(md->prev_free)->next_free = (md->next_free);
	if (md->next_free != NULL)
		(md->next_free)->prev_free = (md->prev_free);
	if (md == free_head)
		free_head = (md->next_free);

	md->prev_free = NULL;
	md->next_free = NULL;
}

void insert_free(md_t * md) {
	//insert front
	md->prev_free = NULL;
	md->next_free = free_head;
	if (free_head != NULL)
		free_head->prev_free = md;
	free_head = md;
}

void split(md_t * md, size_t size) {
	//find address of new md struct to create
	void * new_md_v = (void*)md + sizeof(md_t) + size;
	md_t * new_md = (md_t*)new_md_v;

	//init new_md and update ptrs
	if ((md->next)!= NULL)
		(md->next)->prev = new_md;
	new_md->prev = md;
	new_md->next = (md->next);
	new_md->free = 1;
	new_md->size = md->size - size - sizeof(md_t);

	md->next = new_md;
	md->size = size;
	insert_free(new_md);
}

int coalesce(md_t * md) {
	md_t * next = md->next;
	md_t * prev = md->prev;

	//check if "adj" blocks are actually adjacent
	int prev_adj = 0;
	int next_adj = 0;
	if (prev != NULL && ((void*)prev+sizeof(md_t)+prev->size==(void*)md))
		prev_adj = 1;
	if (next != NULL && ((void*)md+sizeof(md_t)+md->size==(void*)next))
		next_adj = 1;
	//both prev and next are free and actually adj
	if (next!=NULL && next->free==1 && prev!=NULL && prev->free==1 && prev_adj==1 && next_adj==1) {
		remove_free(next);
		md_t * next_next = next->next;
		if (next_next != NULL)
			next_next->prev = prev;
		if (next == used_tail)
			used_tail = prev;
		prev->size += md->size + next->size + 2*sizeof(md_t);
		prev->next = next_next;
		return 1;
	}
	
	//only prev is free and actually adj
	if (prev!=NULL && prev->free==1 && prev_adj) {
		if (next != NULL)
			next->prev = prev;
		if (md == used_tail)
			used_tail = prev;
		prev->size += md->size + sizeof(md_t);
		prev->next = next;
		return 1;
	}

	//only next is free and actually adj
	if (next!=NULL && next->free==1 && next_adj && next!=used_tail) {
		remove_free(next);
		md_t * next_next = next->next;
		if (next_next != NULL)
			next_next->prev = md;
		md->size += next->size + sizeof(md_t);
		md->next = next_next;
		md->free = 1;
		insert_free(md);
		return 1;
	}
	return 0;
}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    // implement calloc!
	if (num==0 || size==0)
		return NULL;

	size_t req_size = num * size;

	void * ptr = malloc(req_size);
	if (ptr == NULL) {
		//fprintf(stderr, "calloc: malloc failed!\n");
		return NULL;
	}
	ptr = memset(ptr, 0, req_size);
    return ptr;
}

/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
void *malloc(size_t size) {
    // implement malloc!
	if (size == 0)
		return NULL;

	md_t * curr = free_head;

	while (curr != NULL) {
		//first fit
		if (curr->size>=size) {
			remove_free(curr);
			if (curr->size >= size + sizeof(md_t) + SPLIT_THRESHOLD) {
				split(curr, size);
			}
			curr->free = 0;
			return (void*)(curr + 1);
		}
		curr = curr->next_free;
	}

	//allocate memory if none available in linked list
	void * chosen_v = sbrk(sizeof(md_t) + size);
	if (chosen_v == (void*)-1) {
		//fprintf(stderr, "malloc: sbrk failed!\n");
		return NULL;
	}
	md_t * chosen = (md_t*) chosen_v;
	chosen->size = size;
	chosen->free = 0;
	chosen->prev_free = NULL;
	chosen->next_free = NULL;
	//insert front
/*
	chosen->next = used_head;
	chosen->prev = NULL;
	if (used_head != NULL)
		used_head->prev = chosen;
	used_head = chosen;
*/
	chosen->prev = used_tail;
	if (used_tail != NULL)
		used_tail->next = chosen;
	used_tail = chosen;
	return (void*)(chosen + 1);
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
    // implement free!
	if (ptr == NULL)
		return;

	md_t * p = (md_t*)ptr - 1; // or ptr - sizeof(md_t);
	if (p->free == 1) {
		//fprintf(stderr, "double free!\n");
		return;
	}
	int coal_res = coalesce(p);
	if (coal_res == 0) {
		p->free = 1;
		insert_free(p);
	}
}

/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
    // implement realloc!
	if (ptr == NULL)
		return malloc(size);

	md_t * md = (md_t*)ptr - 1;
	size_t old_size = md->size;

	if (old_size > 2*size && (old_size-size-sizeof(md_t))>SPLIT_THRESHOLD) {
		split(md, size);
	}

	if (old_size >= size)
		return ptr;

	void * res = malloc(size);
	if (res == NULL) {
		//fprintf(stderr, "realloc: malloc failed!\n");
		return NULL;
	}
	size_t min_size = (size_t)fmin((double)size, (double)old_size);
	//memcpy(res, ptr, old_size);
	memcpy(res, ptr, min_size);
	free(ptr);
    return res;
}
