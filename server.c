#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define WEB_ROOT "www"  // Serve files from "www" directory

// Get MIME type based on file extension
const char* get_mime_type(const char* path) {
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".css")) return "text/css";
    if (strstr(path, ".js")) return "application/javascript";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".jpg") || strstr(path, ".jpeg")) return "image/jpeg";
    return "text/plain";
}

// Serve the requested file
void serve_file(int client_socket, const char* path) {
    char full_path[256];
    snprintf(full_path, sizeof(full_path), "%s%s", WEB_ROOT, path);

    // If the path is a directory, serve index.html
    if (full_path[strlen(full_path) - 1] == '/')
        strcat(full_path, "index.html");

    int file_fd = open(full_path, O_RDONLY);
    if (file_fd < 0) {
        // Send 404 Not Found response
        const char* response = "HTTP/1.1 404 Not Found\r\n"
                               "Content-Type: text/html\r\n"
                               "Content-Length: 23\r\n"
                               "\r\n"
                               "<h1>404 Not Found</h1>";
        write(client_socket, response, strlen(response));
    } else {
        // Get file size
        struct stat file_stat;
        fstat(file_fd, &file_stat);
        int file_size = file_stat.st_size;

        // Send HTTP headers
        char headers[256];
        snprintf(headers, sizeof(headers),
                 "HTTP/1.1 200 OK\r\n"
                 "Content-Type: %s\r\n"
                 "Content-Length: %d\r\n"
                 "\r\n",
                 get_mime_type(full_path), file_size);
        write(client_socket, headers, strlen(headers));

        // Send file content
        char buffer[BUFFER_SIZE];
        int bytes_read;
        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            write(client_socket, buffer, bytes_read);
        }

        close(file_fd);
    }

    close(client_socket);
}

// Handle incoming HTTP request
void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, BUFFER_SIZE);

    // Extract requested file path (e.g., GET /index.html)
    char method[8], path[128];
    sscanf(buffer, "%s %s", method, path);

    // Serve file
    serve_file(client_socket, path);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running on http://localhost:%d\n", PORT);

    while (1) {
        // Accept connection
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // Handle request in child process
        if (fork() == 0) {
            close(server_fd);
            handle_request(client_socket);
            exit(0);
        }

        close(client_socket);
    }

    return 0;
}
