#include "server.h"

int main(int argc, char **argv) {
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;
    int listenfd = -1;
    char *port = "80";
    pthread_t tid;

    if (argc > 1) {
        port = argv[1];
    }

    listenfd = connection(port);

    if (listenfd == -1) {
        fprintf(stderr, "Error: Could not establish server on port %s\n", port);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %s\n", port);

    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        int *confd = malloc(sizeof(int));

        if (confd == NULL) {
            perror("Error allocating memory for client connection");
            continue;
        }

        *confd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

        if (*confd < 0) {
            perror("Error accepting client connection");
            free(confd);
            continue;
        }

        printf("New client connected\n");

        if (pthread_create(&tid, NULL, client_handler, confd) != 0) {
            perror("Error creating thread");
            free(confd);
            continue;
        }
    }

    close(listenfd);
    return 0;
}
