//worked with cb4 this lab

/**
 * Charming Chatroom
 * CS 241 - Spring 2019
 */
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"

#define MAX_CLIENTS 8

void *process_client(void *p);

static volatile int serverSocket;
static volatile int endSession;

static volatile int clientsCount;
static volatile int clients[MAX_CLIENTS];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static struct addrinfo hints;
static struct addrinfo * res;

typedef struct sockaddr sockaddr_t;

/**
 * Signal handler for SIGINT.
 * Used to set flag to end server.
 */
void close_server() {
    endSession = 1;
    // add any additional flags here you want.
	serverSocket = -1;
}

/**
 * Cleanup function called in main after `run_server` exits.
 * Server ending clean up (such as shutting down clients) should be handled
 * here.
 */
void cleanup() {
    if (shutdown(serverSocket, SHUT_RDWR) != 0) {
        perror("shutdown():");
    }
    close(serverSocket);

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != -1) {
            if (shutdown(clients[i], SHUT_RDWR) != 0) {
                perror("shutdown(): ");
            }
            close(clients[i]);
        }
    }
}

/**
 * Sets up a server connection.
 * Does not accept more than MAX_CLIENTS connections.  If more than MAX_CLIENTS
 * clients attempts to connects, simply shuts down
 * the new client and continues accepting.
 * Per client, a thread should be created and 'process_client' should handle
 * that client.
 * Makes use of 'endSession', 'clientsCount', 'client', and 'mutex'.
 *
 * port - port server will run on.
 *
 * If any networking call fails, the appropriate error is printed and the
 * function calls exit(1):
 *    - fprtinf to stderr for getaddrinfo
 *    - perror() for any other call
 */
void run_server(char *port) {
    /*QUESTION 1*/
    /*QUESTION 2*/
    /*QUESTION 3*/

    /*QUESTION 8*/

    /*QUESTION 4*/
    /*QUESTION 5*/
    /*QUESTION 6*/

    /*QUESTION 9*/

    /*QUESTION 10*/
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		perror("socket failed");
		close_server();
		if (res)
			freeaddrinfo(res);
		exit(1);
	}
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM;

	int result = getaddrinfo(NULL, port, &hints, &res);
	if (result != 0) {
		perror(gai_strerror(result));
		close_server();
		if (res)
			freeaddrinfo(res);
		exit(1);
	}

	int optval = 1;
	size_t optlen = sizeof(optval);
	result = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &optval, optlen);
	if (result == -1) {
		perror("setsockopt failed");
		close_server();
		if (res)
			freeaddrinfo(res);
		exit(1);
	}

	result = bind(serverSocket, res->ai_addr, res->ai_addrlen);
	if (result != 0) {
		perror("bind failed");
		close_server();
		if (res)
			freeaddrinfo(res);
		exit(1);
	}

	result = listen(serverSocket, 10);
	if (result != 0) {
		perror("listen failed");
		close_server();
		if (res)
			freeaddrinfo(res);
		exit(1);
	}

	for (int i=0; i<MAX_CLIENTS; i++) {
		clients[i] = -1;
	}

	while (endSession == 0) {
		if (clientsCount < MAX_CLIENTS) {
			sockaddr_t client;
			socklen_t clientlen = sizeof(sockaddr_t);
			memset(&client, 0, sizeof(sockaddr_t));
			int socket_fd = accept(serverSocket, (sockaddr_t *) &clientlen, &clientlen);
			if (endSession != 0)
				break;
			if (socket_fd == -1) {
				perror("accept failed");
				close_server();
				if (res)
					freeaddrinfo(res);
				exit(1);
			}
			intptr_t clientId = -1;

			pthread_mutex_lock(&mutex);
			for(int i=0; i<MAX_CLIENTS; i++) {
				if (clients[i] == -1) {
					clients[i] = socket_fd;
					char ip[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, &client, ip, clientlen);
					clientId = i;
					break;
				}
			}
			clientsCount++;
			pthread_mutex_unlock(&mutex);

			pthread_t pid;
			int pres = pthread_create(&pid, NULL, process_client, (void*) clientId);
			if (pres != 0)
				exit(1);
			puts("waiting for connection..");
		}
	}
	if (serverSocket != -1)
		close_server();
}

/**
 * Broadcasts the message to all connected clients.
 *
 * message  - the message to send to all clients.
 * size     - length in bytes of message to send.
 */
void write_to_clients(const char *message, size_t size) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] != -1) {
            ssize_t retval = write_message_size(size, clients[i]);
            if (retval > 0) {
                retval = write_all_to_socket(clients[i], message, size);
            }
            if (retval == -1) {
                perror("write(): ");
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

/**
 * Handles the reading to and writing from clients.
 *
 * p  - (void*)intptr_t index where clients[(intptr_t)p] is the file descriptor
 * for this client.
 *
 * Return value not used.
 */
void *process_client(void *p) {
    pthread_detach(pthread_self());
    intptr_t clientId = (intptr_t)p;
    ssize_t retval = 1;
    char *buffer = NULL;

    while (retval > 0 && endSession == 0) {
        retval = get_message_size(clients[clientId]);
        if (retval > 0) {
            buffer = calloc(1, retval);
            retval = read_all_from_socket(clients[clientId], buffer, retval);
        }
        if (retval > 0)
            write_to_clients(buffer, retval);

        free(buffer);
        buffer = NULL;
    }

    printf("User %d left\n", (int)clientId);
    close(clients[clientId]);

    pthread_mutex_lock(&mutex);
    clients[clientId] = -1;
    clientsCount--;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "%s <port>\n", argv[0]);
        return -1;
    }

    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = close_server;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        return 1;
    }

    run_server(argv[1]);
    cleanup();
    pthread_exit(NULL);
}
