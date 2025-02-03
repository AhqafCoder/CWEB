#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define WEB_ROOT "www"

// Function to get MIME type
const char* get_mime_type(const char* path) {
    if (strstr(path, ".html")) return "text/html";
    if (strstr(path, ".css")) return "text/css";
    if (strstr(path, ".js")) return "application/javascript";
    if (strstr(path, ".png")) return "image/png";
    if (strstr(path, ".jpg") || strstr(path, ".jpeg")) return "image/jpeg";
    return "text/plain";
}

// Function to decode URL-encoded strings
void urldecode(char *src, char *dest) {
    char *p = src;
    char code[3] = {0};
    while (*p) {
        if (*p == '%') {
            strncpy(code, p + 1, 2);
            *dest++ = (char) strtol(code, NULL, 16);
            p += 3;
        } else if (*p == '+') {
            *dest++ = ' ';
            p++;
        } else {
            *dest++ = *p++;
        }
    }
    *dest = '\0';
}

// Function to serve static files
void serve_file(int client_socket, const char* path) {
    char full_path[256];

    // Serve index.html if root is requested
    if (strcmp(path, "/") == 0) {
        strcpy(full_path, WEB_ROOT "/index.html");
    } else {
        snprintf(full_path, sizeof(full_path), "%s%s", WEB_ROOT, path);
    }

    int file_fd = open(full_path, O_RDONLY);
    if (file_fd < 0) {
        const char* response = "HTTP/1.1 404 Not Found\r\n"
                               "Content-Type: text/html\r\n"
                               "Content-Length: 23\r\n"
                               "\r\n"
                               "<h1>404 Not Found</h1>";
        write(client_socket, response, strlen(response));
        close(client_socket);
        return;
    }

    struct stat file_stat;
    fstat(file_fd, &file_stat);
    int file_size = file_stat.st_size;

    char headers[256];
    snprintf(headers, sizeof(headers),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "\r\n",
             get_mime_type(full_path), file_size);
    
    write(client_socket, headers, strlen(headers));

    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        write(client_socket, buffer, bytes_read);
    }

    close(file_fd);
    close(client_socket);
}

// Function to handle POST request and store student data correctly
void handle_post_request(int client_socket, char* body) {
    char decoded[BUFFER_SIZE] = {0};
    urldecode(body, decoded);  // Decode URL-encoded data

    char name[256] = {0}, id[256] = {0}, roll[256] = {0}, dob[256] = {0};
    char gender[16] = {0}, email[256] = {0}, phone[32] = {0};

    // Extract parameters correctly
    sscanf(decoded, "name=%[^&]&id=%[^&]&roll=%[^&]&dob=%[^&]&gender=%[^&]&email=%[^&]&phone=%s",
           name, id, roll, dob, gender, email, phone);

    // Open file in append mode
    FILE* file = fopen("www/students.txt", "a");
    if (!file) {
        perror("Error opening students.txt");
        return;
    }
    fprintf(file, "%s,%s,%s,%s,%s,%s,%s\n", name, id, roll, dob, gender, email, phone);
    fclose(file);

    // Response after successful submission
    const char* response = "HTTP/1.1 200 OK\r\n"
                           "Content-Type: text/html\r\n"
                           "\r\n"
                           "<h1>Student Added Successfully!</h1>"
                           "<a href='/view-students.html'>View Students</a>";
    write(client_socket, response, strlen(response));
    close(client_socket);
}

// Function to handle HTTP requests
void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, BUFFER_SIZE);

    char method[8], path[128];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(method, "GET") == 0) {
        serve_file(client_socket, path);
    } else if (strcmp(method, "POST") == 0 && strstr(path, "/add-student")) {
        char* body = strstr(buffer, "\r\n\r\n") + 4;  // Extract body
        handle_post_request(client_socket, body);
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server running on http://localhost:%d\n", PORT);

    while (1) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }

        if (fork() == 0) {
            close(server_fd);
            handle_request(client_socket);
            exit(0);
        }
        close(client_socket);
    }

    return 0;
}
