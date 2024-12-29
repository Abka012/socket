#include "client.h"

int main() {
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char *hello = "Hello from client";

    // Create socket
    client_fd = create_socket();

    // Configure server address
    configure_server_address(&serv_addr);

    // Connect to the server
    if (connect_to_server(client_fd, &serv_addr) < 0) {
        return -1;
    }

    // Send message to server
    send_message(client_fd, hello);

    // Read response from server
    read_response(client_fd, buffer, sizeof(buffer));
    printf("%s\n", buffer);

    // Close the socket
    close(client_fd);

    return 0;
}
