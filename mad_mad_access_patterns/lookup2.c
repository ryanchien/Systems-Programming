//worked with cb4

/**
 * Mad Mad Access Patterns
 * CS 241 - Spring 2019
 */
#include "tree.h"
#include "utils.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses mmap to access the data.

  ./lookup2 <data_file> <word> [<word> ...]
*/

void lookup(void * mapped, size_t offset, char * word) {
	if (offset <= 0) {
		printNotFound(word);
		return;
	}
	BinaryTreeNode * node = (BinaryTreeNode*) (mapped+offset);
	int res = strcmp(word, node->word);
	if (res == 0) {
		printFound(word, node->count, node->price);
		return;
	}
	else if (res > 0)
		lookup(mapped, node->right_child, word);
	else
		lookup(mapped, node->left_child, word);
}

int main(int argc, char **argv) {
	if (argc <= 2) {
		printArgumentUsage();
		exit(1);
	}
	
	char * data_file = argv[1];
	int fd = open(data_file, O_RDONLY);
	if (fd == -1) {
		openFail(data_file);
		exit(2);
	}

	struct stat statbuf;
	int res = fstat(fd, &statbuf);
	if (res == -1) {
		openFail(data_file);
		exit(2);
	}
	res = S_ISREG(statbuf.st_mode);
	if (res == 0) {
		openFail(data_file);
		exit(2);
	}

	void * mapped = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (mapped == MAP_FAILED) {
		mmapFail(data_file);
		exit(2);
	}

	res = close(fd);
	if (res == -1) {
		openFail(data_file);
		exit(2);
	}
	if(strncmp(mapped, "BTRE", 4) != 0) {
		formatFail(data_file);
		exit(2);
	}

	for (int i=2; i<argc; i++) {
		lookup(mapped, 4, argv[i]);
	}
    return 0;
}
