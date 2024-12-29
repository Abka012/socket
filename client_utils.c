#include "client.h"
#include "helper.h"

char prompt[] = "Chatroom> ";

// Print usage information
void usage() {
    printf("-h  print help\n");
    printf("-a  IP address of the server [Required]\n");
    printf("-p  port number of the server [Required]\n");
    printf("-u  enter your username [Required]\n");
}

// Establish a connection to the server
int connection(char* hostname, char* port) {
    int clientfd, rc;
    struct addrinfo hints, *listp, *p;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // Connections only
    hints.ai_flags |= AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV; // Use fixed port number

    if ((rc = getaddrinfo(hostname, port, &hints, &listp)) != 0) {
        fprintf(stderr, "Invalid hostname or port number\n");
        return -1;
    }

    for (p = listp; p; p = p->ai_next) {
        clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (clientfd < 0) {
            continue; // Socket creation failed, try the next
        }

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1) {
            break; // Success
        }

        if (close(clientfd) < 0) {
            fprintf(stderr, "connection: close failed: %s\n", strerror(errno));
            return -1;
        }
    }

    freeaddrinfo(listp);
    if (!p) {
        return -1; // All connection attempts failed
    } else {
        return clientfd; // Connection succeeded
    }
}

// Thread function for reading server responses
void reader(void* var) {
    char buf[MAXLINE];
    io_t io;
    int status;
    int connID = (int)(intptr_t)var;

    io_readinitb(&io, connID);
    while (1) {
        while ((status = io_readlineb(&io, buf, MAXLINE)) > 0) {
            if (status == -1) {
                exit(1);
            }

            if (!strcmp(buf, "\r\n")) {
                break;
            }

            if (!strcmp(buf, "exit")) {
                close(connID);
                exit(0);
            }

            if (!strcmp(buf, "start\n")) {
                printf("\n");
            } else {
                printf("%s", buf);
            }
        }

        printf("%s", prompt);
        fflush(stdout);
    }
}
