/**
 * Mini Valgrind Lab
 * CS 241 - Spring 2019
 */
 
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Your tests here using malloc and free

	void * p1 = malloc(30);
	free(p1);

	void * p2 = calloc(5, 11);
	free(p2);
	
	void * p3 = malloc(30);
	p3 = realloc(p3, 25);
	free(p3);

	void * p4 = malloc(30);
	p4 = realloc(p4, 35);
	free(p4);

	void * p5 = malloc(30);
	p5 = realloc(p5, 35);
	free(p5);

	void * p6 = NULL;
	free(p6);

	void * p7 = malloc(25);
	void * p8 = calloc(10,10);
	void * p9 = malloc(30);
	free(p9);
	free(p8);
	free(p7);

	//invalid
	free(p7);

	//leak
	void * p10 = malloc(100);
	p10 = realloc(p10, 50);

/*
	void * p1 = malloc(30);
	void * p2 = malloc(40);
	void * p3 = malloc(50);
	free(p2);
*/
    return 0;
}
