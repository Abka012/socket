// client.h

#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

// Function prototypes
int create_socket();
int configure_server_address(struct sockaddr_in *serv_addr);
int connect_to_server(int client_fd, struct sockaddr_in *serv_addr);
void send_message(int client_fd, const char *message);
void read_response(int client_fd, char *buffer, size_t buffer_size);

#endif // CLIENT_H
