// Standard C libraries for networking, signal handling, input/output, string operations, and system processes
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

// Custom project headers for authentication and ASCII graphics
#include "auth.h"
#include "Look.h"

// Helper function: Removes invisible newline ('\n') and carriage return ('\r') 
// characters from the end of user input (like when pressing Enter in netcat)
static void trim_line(char *str)
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0'; // Replace newline with string terminator
        len--;
    }
}

// Main function that sets up and runs the TCP server
void start_server(void)
{
    // Variables for server and client address information
    struct sockaddr_in server_addr, client_addr;
    int server_fd, new_socket;
    socklen_t client_len = sizeof(client_addr);
    int opt = 1;
    char recv_data[1024]; // Buffer to store incoming data from client
    char send_data[1024]; // Buffer to format outgoing data to client

    // Zero out the server address structure to clean any random memory garbage
    memset(&server_addr, 0, sizeof(server_addr));

    // Configure server settings: IPv4, Port 8080, Accept connections from any network interface
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // STEP 1: Create a TCP socket (IPv4, Streaming socket)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // STEP 2: Allow immediate reuse of the port after server restart (prevents "Address already in use" errors)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // STEP 3: Bind the created socket to port 8080
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Bind successful on port 8080\n");

    // STEP 4: Put socket into listening mode with a queue limit of 5 pending connections
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    // Automatically cleanup finished child processes to avoid "zombie" processes taking system memory
    signal(SIGCHLD, SIG_IGN);

    // STEP 5: Main server loop — continuously accept new incoming client connections
    while (1) {
        // Pause and wait for a client to connect
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
        if (new_socket < 0) {
            perror("accept failed");
            continue;
        }

        printf("Client connected!\n");

        // STEP 6: Duplicate process using fork() so every client gets its own isolated worker
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            close(new_socket);
            continue;
        }

        // --- CHILD PROCESS CODE (Runs inside each client's isolated copy) ---
        if (pid == 0) {
            // Child doesn't need the main listening socket, so close it
            close(server_fd);

            int authenticated = 0;
            char username[128] = {0};
            char password[128] = {0};

            // Send initial welcome message to newly connected client
            const char *welcome = "Connected to Marie Server.\nType 'loginshell' to authenticate.\n> ";
            send(new_socket, welcome, strlen(welcome), 0);

            // Client interaction loop (handles login and commands)
            while (1) {
                memset(recv_data, 0, sizeof(recv_data));
                
                // Receive incoming data from the client
                ssize_t bytes_received = recv(new_socket, recv_data, sizeof(recv_data) - 1, 0);

                // If client disconnects or an error occurs, exit loop
                if (bytes_received <= 0) {
                    printf("Client disconnected or recv error\n");
                    break;
                }

                recv_data[bytes_received] = '\0'; // Ensure string ends properly
                trim_line(recv_data);             // Clean trailing \r\n characters

                // Handle quit command
                if (strcmp(recv_data, "quit") == 0) {
                    printf("Client requested to quit\n");
                    break;
                }

                // UNAUTHENTICATED STATE: Force client to type 'loginshell' and supply credentials
                if (!authenticated) {
                    if (strcmp(recv_data, "loginshell") == 0) {
                        // 1. Prompt and read username
                        const char *u_prompt = "Username: ";
                        send(new_socket, u_prompt, strlen(u_prompt), 0);

                        memset(username, 0, sizeof(username));
                        ssize_t u_bytes = recv(new_socket, username, sizeof(username) - 1, 0);
                        if (u_bytes <= 0) break;
                        username[u_bytes] = '\0';
                        trim_line(username);

                        // 2. Prompt and read password
                        const char *p_prompt = "Password: ";
                        send(new_socket, p_prompt, strlen(p_prompt), 0);

                        memset(password, 0, sizeof(password));
                        ssize_t p_bytes = recv(new_socket, password, sizeof(password) - 1, 0);
                        if (p_bytes <= 0) break;
                        password[p_bytes] = '\0';
                        trim_line(password);

                        // 3. Verify credentials against users.txt
                        if (check_credentials(username, password)) {
                            authenticated = 1; // Mark client as authenticated
                            const char *ok_msg = "Login successful!\n\n";
                            send(new_socket, ok_msg, strlen(ok_msg), 0);

                            // Send ASCII banner and user details to screen
                            send_ascii_logo(new_socket, username, password);
                        } else {
                            const char *err_msg = "Invalid username or password. Type 'loginshell' to try again.\n> ";
                            send(new_socket, err_msg, strlen(err_msg), 0);
                        }
                    } else {
                        const char *gate_msg = "Access Denied. Please type 'loginshell' to log in.\n> ";
                        send(new_socket, gate_msg, strlen(gate_msg), 0);
                    }
                    continue; // Loop back for next input
                }

                // --- AUTHENTICATED STATE: Process commands after successful login ---
                snprintf(send_data, sizeof(send_data), "ACK: %.900s\n> ", recv_data);
                send(new_socket, send_data, strlen(send_data), 0);
            }

            // Close connection and exit child process when done
            close(new_socket);
            exit(0);
        } 
        // --- PARENT PROCESS CODE ---
        else 
        {
            // Parent closes client socket copy (child handles it) and loops back to accept() next client
            close(new_socket);
        }
    }

    // Close server listening socket (reached only on system shutdown)
    close(server_fd);
}