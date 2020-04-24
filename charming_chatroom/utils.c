//worked with cb4 this lab

/**
 * Charming Chatroom
 * CS 241 - Spring 2019
 */
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
static const size_t MESSAGE_SIZE_DIGITS = 4;

char *create_message(char *name, char *message) {
    int name_len = strlen(name);
    int msg_len = strlen(message);
    char *msg = calloc(1, msg_len + name_len + 4);
    sprintf(msg, "%s: %s", name, message);

    return msg;
}

ssize_t get_message_size(int socket) {
    int32_t size;
    ssize_t read_bytes =
        read_all_from_socket(socket, (char *)&size, MESSAGE_SIZE_DIGITS);
    if (read_bytes == 0 || read_bytes == -1)
        return read_bytes;

    return (ssize_t)ntohl(size);
}

// You may assume size won't be larger than a 4 byte integer
ssize_t write_message_size(size_t size, int socket) {
    // Your code here
	int32_t m_size = htonl(size);
	ssize_t wr_bytes = write_all_to_socket(socket, (char*)&m_size, MESSAGE_SIZE_DIGITS);
    return wr_bytes;
}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
    // Your Code Here
	ssize_t bytes_read = 0;
	ssize_t bytes_left = count;
	ssize_t res = 0;

	while (res==-1 || bytes_left > 0) {
		res = read(socket, (void*) (buffer+bytes_read), bytes_left);
		if (res == 0)
			return 0;
		if (res > 0) {
			bytes_read += res;
			bytes_left -= res;
		}
	}
    return count;
}

ssize_t write_all_to_socket(int socket, const char *buffer, size_t count) {
    // Your Code Here
	ssize_t bytes_read = 0;
	ssize_t bytes_left = count;
	ssize_t res = 0;

	while (res==-1 || bytes_left > 0) {
		res = write(socket, (void*) (buffer+bytes_read), bytes_left);
		if (res == 0)
			return 0;
		if (res > 0) {
			bytes_read += res;
			bytes_left -= res;
		}
	}
    return count;
}
