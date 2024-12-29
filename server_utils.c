#include "server.h"

int create_server_socket() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void configure_server_address(struct sockaddr_in *address) {
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(PORT);
}

void bind_socket(int server_fd, struct sockaddr_in *address) {
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

void start_listening(int server_fd) {
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
}

int accept_connection(int server_fd, struct sockaddr_in *address, socklen_t *addrlen) {
    int client_socket = accept(server_fd, (struct sockaddr *)address, addrlen);
    if (client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    return client_socket;
}

void handle_client(int client_socket) {
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    ssize_t valread = read(client_socket, buffer, sizeof(buffer) - 1);
    if (valread >= 0) {
        buffer[valread] = '\0'; // Null-terminate the buffer
        printf("Received: %s\n", buffer);
    } else {
        perror("Read failed");
    }

    send(client_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
}
