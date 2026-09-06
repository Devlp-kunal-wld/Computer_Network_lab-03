#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Same computer = localhost
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        exit(1);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr,
                sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    printf("Connected to server.\n");

    // Chat loop
    while (1) {
        printf("Client: ");

        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        send(sock, message, strlen(message), 0);

        // Stop when client sends bye
        if (strcmp(message, "bye") == 0) {
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);

        int n = recv(sock, buffer, BUFFER_SIZE - 1, 0);

        if (n <= 0) {
            break;
        }

        buffer[n] = '\0';

        printf("Server: %s\n", buffer);
    }

    close(sock);

    return 0;
}
