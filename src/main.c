#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void)
{
    struct sockaddr_in server_addr;
    int server_fd, new_socket;

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;          // IPv4
    server_addr.sin_port = htons(8080);        // Port 8080, network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections on any local interface

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Bind successful on port 8080\n");

    close(server_fd);
    return 0;
}