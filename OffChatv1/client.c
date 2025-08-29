#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Type messages...\n");

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Send message to server
        send(sockfd, buffer, strlen(buffer), 0);

        // Exit condition
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Disconnected.\n");
            break;
        }

        // Receive response
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(sockfd, buffer, sizeof(buffer), 0);
        if (valread > 0) {
            printf("Server: %s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
