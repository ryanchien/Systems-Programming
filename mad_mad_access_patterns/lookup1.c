//worked with cb4

/**
 * Mad Mad Access Patterns
 * CS 241 - Spring 2019
 */
#include "tree.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
*/

void lookup(char * data_file, FILE * fp, size_t offset, char * word) {
	if (offset <= 0) {
		printNotFound(word);
		return;
	}

	int res = fseek(fp, offset, SEEK_SET);
	if (res == -1) {
		formatFail(data_file);
		exit(2);
	}

	size_t size = sizeof(BinaryTreeNode);
	BinaryTreeNode node;
	fread(&node, size, 1, fp);
	res = fseek(fp, offset + size, SEEK_SET);
	if (res == -1) {
		formatFail(data_file);
		exit(2);
	}

	char buf[256];
	char * fgres = fgets(buf, 256, fp);
	if (fgres == NULL) {
		formatFail(data_file);
		exit(2);
	}

	res = strcmp(word, buf);
	if (res == 0) {
		printFound(word, node.count, node.price);
		return;
	}
	else if (res > 0)
		lookup(data_file, fp, node.right_child, word);
	else
		lookup(data_file, fp, node.left_child, word);
}

int main(int argc, char **argv) {
	if (argc <= 2) {
		printArgumentUsage();
		exit(1);
	}

	char * data_file = argv[1];
	FILE * fp = fopen(data_file, "r");
	if (fp == NULL) {
		openFail(data_file);
		exit(2);
	}

	char btre[5];
	fread(btre, 4, 1, fp);
	btre[4] = '\0';
	if (strcmp(btre, "BTRE") != 0) {
		formatFail(data_file);
		exit(2);
	}

	for (int i=2; i<argc; i++) {
		lookup(data_file, fp, 4, argv[i]);
	}

	fclose(fp);
    return 0;
}
