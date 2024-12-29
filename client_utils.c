#include "client.h"

int create_socket() {
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }
    return client_fd;
}

int configure_server_address(struct sockaddr_in *serv_addr) {
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr->sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return -1;
    }
    return 0;
}

int connect_to_server(int client_fd, struct sockaddr_in *serv_addr) {
    if (connect(client_fd, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0) {
        perror("Connection failed");
        return -1;
    }
    return 0;
}

void send_message(int client_fd, const char *message) {
    send(client_fd, message, strlen(message), 0);
    printf("Message sent: %s\n", message);
}

void read_response(int client_fd, char *buffer, size_t buffer_size) {
    int valread = read(client_fd, buffer, buffer_size - 1);
    if (valread >= 0) {
        buffer[valread] = '\0'; // Null-terminate the buffer
    } else {
        perror("Read failed");
    }
}
