/**
 * Perilous Pointers Lab
 * CS 241 - Spring 2019
 */
 
#include "part2-functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
	//1
	first_step(81);
	//2
	int i = 132;
	second_step(&i);
	//3
	i = 8942;
	int * ptr = &i;
	double_step(&ptr);
	//4
	char ss[9] = {0,1,2,3,4,15,0,0,0};
	strange_step(ss);
	/*
	also works
	int x = 15;
	char * p = (char*) &x;
	strange_step(p-5);
	*/
	//5
	empty_step("\0\0\0\0");
	//6
	two_step("uuuu","uuuu");
	//7
	three_step("abcde","cde","e");
	//8
	step_step_step("aaaa","iiii","qqqq");
	//9
	it_may_be_odd("a", 97);
	//10
	char * ts = malloc(10);
	strcpy(ts, "a,CS241,b");
	tok_step(ts);
	free(ts);
	//11
	char te[4] = {1,0,0,2};
	void *o = te;
	void *b = te;
	the_end(o,b);
    return 0;
}
