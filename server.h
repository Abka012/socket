// server.h

#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1000

// Struct representing a client
struct client {
    char *name;
    int confd;
    struct client *next;
};

// Global variables
extern pthread_mutex_t mutex;
extern struct client *header;

void add_user(struct client *user);
void delete_user(int confd);
int connection(char *port);
void send_msg(int confd, char *msg, char *receiver, char *sender);
void evaluate(char *buf, int confd, char *username);
void *client_handler(void *vargp);

#endif // SERVER_H
