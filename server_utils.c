#include "server.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct client *header = NULL;

// Add a new user to the client list
void add_user(struct client *user) {
    pthread_mutex_lock(&mutex);

    // Add user to the linked list
    user->next = header;
    header = user;

    pthread_mutex_unlock(&mutex);
}

// Delete a user from the client list based on their connection descriptor
void delete_user(int confd) {
    pthread_mutex_lock(&mutex);

    struct client *current = header;
    struct client *previous = NULL;

    // Find the user with the specified connection descriptor
    while (current != NULL && current->confd != confd) {
        previous = current;
        current = current->next;
    }

    // If user is found, remove it from the list
    if (current != NULL) {
        if (previous == NULL) {
            // User is the first in the list
            header = current->next;
        } else {
            // User is not the first in the list
            previous->next = current->next;
        }

        // Free memory for the user
        free(current->name);
        free(current);
    }

    pthread_mutex_unlock(&mutex);
}

// Create a socket and bind to a specific port
int connection(char *port) {
    int sockfd;
    struct addrinfo hints, *res;
    int status;

    // Prepare the hints for getaddrinfo
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get address info for the given port
    if ((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
        perror("getaddrinfo");
        return -1;
    }

    // Create socket
    if ((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("socket");
        freeaddrinfo(res);
        return -1;
    }

    // Bind socket to the port
    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        close(sockfd);
        freeaddrinfo(res);
        return -1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 10) == -1) {
        perror("listen");
        close(sockfd);
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);
    return sockfd;
}

// Send a message to a specific client
void send_msg(int confd, char *msg, char *receiver, char *sender) {
    char buffer[BUFSIZE];
    snprintf(buffer, sizeof(buffer), "%s: %s\n", sender, msg);

    // Send the message to the receiver
    if (send(confd, buffer, strlen(buffer), 0) == -1) {
        perror("send");
    }
}

// Evaluate and process incoming commands
void evaluate(char *buf, int confd, char *username) {
    // Example implementation: process a simple command
    if (strncmp(buf, "/quit", 5) == 0) {
        printf("%s disconnected.\n", username);
        delete_user(confd);
        close(confd);
    } else {
        printf("Message from %s: %s", username, buf);
    }
}

// Client handler thread function
void *client_handler(void *vargp) {
    int confd = *((int *)vargp);
    char buf[BUFSIZE];
    char username[100];

    // Ask for the username
    if (recv(confd, username, sizeof(username), 0) <= 0) {
        perror("recv");
        close(confd);
        return NULL;
    }

    // Create a new client structure and add to the list
    struct client *new_client = (struct client *)malloc(sizeof(struct client));
    new_client->name = strdup(username);
    new_client->confd = confd;
    new_client->next = NULL;
    add_user(new_client);

    printf("User %s connected.\n", username);

    // Main communication loop
    while (1) {
        int bytes_received = recv(confd, buf, sizeof(buf), 0);
        if (bytes_received <= 0) {
            break;  // Client disconnected
        }

        buf[bytes_received] = '\0';  // Null-terminate the buffer
        evaluate(buf, confd, username);
    }

    // Clean up after the client disconnects
    delete_user(confd);
    close(confd);
    printf("User %s disconnected.\n", username);

    return NULL;
}
