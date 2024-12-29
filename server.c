#include "server.h"

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    // Create socket
    server_fd = create_server_socket();

    // Configure server address
    configure_server_address(&address);

    // Bind socket to the configured address and port
    bind_socket(server_fd, &address);

    // Start listening for connections
    start_listening(server_fd);
    printf("Server is listening on port %d\n", PORT);

    // Accept a connection from a client
    client_socket = accept_connection(server_fd, &address, &addrlen);

    // Handle communication with the client
    handle_client(client_socket);

    // Close the sockets
    close(client_socket);
    close(server_fd);

    return 0;
}

