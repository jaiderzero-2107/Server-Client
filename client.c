#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 256

int main(void) {

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed with error: %d\n", result);
        return 1;
    }

    // Create a socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    // Convert IPv4 address from text to binary form
    result = inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    if (result <= 0) {
        if (result == 0)
            printf("Not a valid IP address string\n");
        else
            printf("inet_pton failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == SOCKET_ERROR) {
        printf("connect failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("Enter commands (e.g., TIME, STATUS, ECHO hello, EXIT).\n");

    char command[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    int bytes_received;

    // Main loop to send commands and receive responses
    while (1) {
        printf("\nEnter command: ");

        // Read command
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        command[strcspn(command, "\r\n")] = 0;

        // Send the command to the server
        if (send(client_socket, command, (int)strlen(command), 0) == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            break;
        }

        // EXIT
        if (strcmp(command, "EXIT") == 0) {
             printf("Sent EXIT command. Waiting for server confirmation...\n");
        }

        // Receive the response from the server
        memset(response, 0, sizeof(response));
        bytes_received = recv(client_socket, response, BUFFER_SIZE - 1, 0);

        if (bytes_received > 0) {
            response[bytes_received] = '\0';
            printf("Server response: %s\n", response);
        } else if (bytes_received == 0) {
            printf("Server closed the connection.\n");
            break;
        } else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            break;
        }

        // Break the loop after receiving confirmation for EXIT
        if (strcmp(command, "EXIT") == 0) {
            break;
        }
    }

    // Close the socket and cleanup
    closesocket(client_socket);
    WSACleanup();
    printf("Connection closed. Exiting.\n");

    return 0;
}