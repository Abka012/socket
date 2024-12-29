// server.h

#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

// Function prototypes
int create_server_socket();
void configure_server_address(struct sockaddr_in *address);
void bind_socket(int server_fd, struct sockaddr_in *address);
void start_listening(int server_fd);
int accept_connection(int server_fd, struct sockaddr_in *address, socklen_t *addrlen);
void handle_client(int client_socket);

#endif // SERVER_H
