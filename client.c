#include "client.h"
#include "helper.h"

int main(int argc, char** argv) {
    char* address = NULL;
    char* port = NULL;
    char* username = NULL;
    char cmd[MAXLINE];
    char c;
    pthread_t tid;

    // Parse command-line arguments
    while ((c = getopt(argc, argv, "hu:a:p:u:")) != EOF) {
        switch (c) {
            case 'h':
                usage();
                exit(1);
                break;
            case 'a':
                address = optarg;
                break;
            case 'p':
                port = optarg;
                break;
            case 'u':
                username = optarg;
                break;
            default:
                usage();
                exit(1);
        }
    }

    if (optind == 1 || port == NULL || address == NULL || username == NULL) {
        printf("Invalid usage\n");
        usage();
        exit(1);
    }

    // Establish connection to the server
    int connID = connection(address, port);
    if (connID == -1) {
        printf("Couldn't connect to the server\n");
        exit(1);
    }

    // Add a newline to the username and send it to the server
    char usernameWithNewline[MAXLINE];
    snprintf(usernameWithNewline, sizeof(usernameWithNewline), "%s\n", username);
    if (io_writen(connID, usernameWithNewline, strlen(usernameWithNewline)) == -1) {
        perror("Not able to send the data");
        close(connID);
        exit(1);
    }

    // Create a thread for reading server responses
    pthread_create(&tid, NULL, (void* (*)(void*))reader, (void*)(intptr_t)connID);

    // Print the Chatroom prompt
    printf("%s", prompt);

    // Main loop for sending user input to the server
    while (1) {
        if ((fgets(cmd, MAXLINE, stdin) == NULL) && ferror(stdin)) {
            perror("fgets error");
            close(connID);
            exit(1);
        }

        if (io_writen(connID, cmd, strlen(cmd)) == -1) {
            perror("Not able to send the data");
            close(connID);
            exit(1);
        }
    }
}
