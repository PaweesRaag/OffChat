#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define PORT 8080

int main() {
    int sockfd, clientfd[MAX_CLIENTS];
    char buffer[1024];
    struct sockaddr_in addr, cliaddr;

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // bind
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(sockfd, 100) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on 127.0.0.1:%d...\n", PORT);

    // initialize all client_socket[] to 0
    for (int i = 0; i < MAX_CLIENTS; i++) {
        clientfd[i] = 0;
    }

    fd_set readfds;
    int max_sd;

    while (1) {
        FD_ZERO(&readfds);

        // add server socket
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        // add client sockets
        int sd;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clientfd[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }

        // wait for activity
        if (select(max_sd + 1, &readfds, NULL, NULL, NULL) < 0 && (errno != EINTR)) {
            perror("select error");
        }

        // new connection
        if (FD_ISSET(sockfd, &readfds)) {
            socklen_t addrlen = sizeof(cliaddr);
            int new_sockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &addrlen);
            if (new_sockfd < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection: socket fd = %d , ip = %s , port = %d\n",
                   new_sockfd, inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

            // add new socket to array
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clientfd[i] == 0) {
                    clientfd[i] = new_sockfd;
                    printf("Adding to list of sockets at index %d\n", i);
                    break;
                }
            }
        }

        // IO for each client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sd = clientfd[i];

            if (FD_ISSET(sd, &readfds)) {
                int valread = read(sd, buffer, sizeof(buffer));
                if (valread == 0) {
                    // disconnected
                    socklen_t clilen = sizeof(cliaddr);
                    getpeername(sd, (struct sockaddr *)&cliaddr, &clilen);
                    printf("Host disconnected: ip %s, port %d\n",
                           inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));

                    close(sd);
                    clientfd[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    printf("Message from client %d: %s\n", i, buffer);
                    send(sd, buffer, strlen(buffer), 0); // echo back
                }
            }
        }
    }

    return 0;
}
