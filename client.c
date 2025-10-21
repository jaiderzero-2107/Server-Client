#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "Ws2_32.lib")

int main(void)
{
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
    server_address.sin_port = htons(8080); // Port 8080

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
    result = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (result == SOCKET_ERROR) {
        printf("connect failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Connected to server.\n");

    // Receive data from the server
    char message[256];
    memset(message, 0, sizeof(message)); // Clear the buffer

    // Receive max of 255 characters from server
    int bytes_received = recv(client_socket, message, sizeof(message) - 1, 0);
    if (bytes_received > 0) {
        printf("Received message: %s\n", message);
    } else if (bytes_received == 0) {
        printf("Connection closed by server.\n");
    } else {
        printf("recv failed with error: %d\n", WSAGetLastError());
    }

    // Close the socket and cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
