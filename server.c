
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Accept client
    new_socket = accept(server_fd,
                        (struct sockaddr *)&address,
                        (socklen_t *)&addrlen);

    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected.\n");

    // Chat loop
    while (1) {

        memset(buffer, 0, BUFFER_SIZE);

        int n = recv(new_socket, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0) {
            break;
        }

        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        // Check bye
        if (strcmp(buffer, "bye") == 0) {
            printf("closing connections\n");
            break;
        }

        // Server reply
        printf("Server: ");
        fgets(message, BUFFER_SIZE, stdin);

        message[strcspn(message, "\n")] = '\0';

        send(new_socket, message, strlen(message), 0);
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
