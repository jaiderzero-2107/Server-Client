#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

// Link with the Ws2_32.lib library
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
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind the socket
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080); // Port 8080
    address.sin_addr.s_addr = INADDR_ANY;

    result = bind(server_socket, (struct sockaddr *)&address, sizeof(address));
    if (result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    // Listen for clients (4 allowed)
    result = listen(server_socket, 4);
    if (result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port 8080...\n");

    // Accept a client connection
    SOCKET client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    printf("Client connected.\n");

    // Close the socket
    closesocket(server_socket);

    // Send a message
    char message[] = "Hello, World!";
    result = send(client_socket, message, (int)strlen(message), 0);
    if (result == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    printf("Sent: %s\n", message);

    // Close the client socket and cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}