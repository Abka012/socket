// client.h

#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <netdb.h>

#define MAXLINE 1024

// Global variables
extern char prompt[];

// Function prototypes
void usage();
int connection(char* hostname, char* port);
void reader(void* input);

#endif // CLIENT_H
