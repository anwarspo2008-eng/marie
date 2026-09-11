#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
    struct sockaddr_in server_addr, client_addr;
    int server_fd, new_socket;
    socklen_t client_len = sizeof(client_addr);
    int opt = 1;
    char recv_data[1024];
    char send_data[1024];

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Bind successful on port 8080\n");

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    signal(SIGCHLD, SIG_IGN);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected!\n");

        pid_t pid = fork();

        if (pid < 0) {
            
            perror("fork failed");
            close(new_socket);
            continue;
        }

        if (pid == 0) {
            
            close(server_fd); 

            ssize_t bytes_received = recv(new_socket, recv_data, sizeof(recv_data) - 1, 0);
            if (bytes_received <= 0) {
                printf("Client disconnected or recv error\n");
                close(new_socket);
                exit(0);
            }

            recv_data[bytes_received] = '\0';
            printf("Received: %s", recv_data);

            strcpy(send_data, "Message received");
            ssize_t bytes_sent = send(new_socket, send_data, strlen(send_data), 0);
            printf("Bytes sent: %zd", bytes_sent);

            close(new_socket);
            exit(0);  
        } else {
            
            close(new_socket);  
        }
    }

    close(server_fd);
    return 0;
}