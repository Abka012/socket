#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// run python3 -m http.server 8080 in the terminal  

int createTCPIp4Socket();

int main() {
    int socketFD;
    struct sockaddr_in address;

    socketFD = createTCPIp4Socket();
    if (socketFD < 0) {
        perror("Socket creation failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(8080); 
    address.sin_addr.s_addr = inet_addr( "127.0.0.1");         
    
    int result = connect(socketFD, (const struct sockaddr *)&address, sizeof(address));
    if (result < 0) {
        perror("Connection failed");
        return 1;
    }

    printf("Connected successfully!\n");

    char* msg;
    msg = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
    send(socketFD, msg, strlen(msg), 0);

    char buffer[1024];
    recv(socketFD, buffer, 1024, 0);

    printf("Respond was %s\n", buffer);
    
    return 0;
}

int createTCPIp4Scoket(){
    return socket(AF_INET, SOCK_STREAM, 0);
}
