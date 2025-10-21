#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 1024

// Global file pointer for logging
FILE* log_file;

// Function to log messages
void log_message(const char* message) {

    printf("%s\n", message);

    if (log_file != NULL) {
        fprintf(log_file, "%s\n", message);
        fflush(log_file); // Ensure the log is written immediately
    }
}

// Client session
void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    char log_entry[BUFFER_SIZE * 2];
    int bytes_received;

    // Commands
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the received data

        buffer[strcspn(buffer, "\r\n")] = 0;

        // Log the received command
        snprintf(log_entry, sizeof(log_entry), "[REQUEST] Received command: \"%s\"", buffer);
        log_message(log_entry);

        // --- Command Processing ---

        // 1. TIME
        if (strcmp(buffer, "TIME") == 0) {
            time_t now = time(NULL);
            struct tm* t = localtime(&now);
            strftime(response, sizeof(response), "Current time: %H:%M:%S", t);
        }
        // 2. STATUS
        else if (strcmp(buffer, "STATUS") == 0) {
            strcpy(response, "Server active and awaiting connections");
        }
        // 3. ECHO <message>
        else if (strncmp(buffer, "ECHO ", 5) == 0) {
            // The response is the part of the buffer after "ECHO "
            strcpy(response, buffer + 5);
        }
        // 4. EXIT
        else if (strcmp(buffer, "EXIT") == 0) {
            strcpy(response, "Connection closed");
        }
        // 5. Unknown/Error command
        else {
            strcpy(response, "Error: Unknown command");
        }

        // Response
        send(client_socket, response, (int)strlen(response), 0);

        // Log the response
        snprintf(log_entry, sizeof(log_entry), "[RESPONSE] Sent: \"%s\"", response);
        log_message(log_entry);

        // EXIT to break the loop
        if (strcmp(buffer, "EXIT") == 0) {
            break;
        }
    }

    if (bytes_received == 0) {
        log_message("Client disconnected.");
    } else if (bytes_received == SOCKET_ERROR) {
        log_message("recv failed, client have disconnected.");
    }

    closesocket(client_socket);
    log_message("Connection closed.");
}


int main(void) {

    // Open log file
    log_file = fopen("server_log.txt", "a");
    if (log_file == NULL) {
        printf("Error: Could not open log file.\n");
        return 1;
    }

    log_message("--------------------");
    log_message("Server starting...");

    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        log_message("WSAStartup failed.");
        return 1;
    }

    // Create listening socket
    SOCKET listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET) {
        log_message("socket creation failed.");
        WSACleanup();
        return 1;
    }

    // Bind the socket
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    result = bind(listen_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    // Listen for clients (4 allowed)
    result = listen(listen_socket, 4);
    if (result == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listen_socket);
        WSACleanup();
        return 1;
    }

    char message[100];
    snprintf(message, sizeof(message), "Server started on port %d. Waiting for connections...", PORT);
    log_message(message);

    // Main server loop to accept new clients
    while (1) {
        SOCKET client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) {
            log_message("accept failed.");
            continue;
        }

        log_message("Client connected.");
        handle_client(client_socket); // Client session is in this function
    }

    // Close the client socket and cleanup
    closesocket(listen_socket);
    WSACleanup();
    fclose(log_file);
    return 0;
}