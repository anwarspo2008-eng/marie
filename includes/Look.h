#ifndef LOOK_H
#define LOOK_H

/*
 * Sends the ASCII art logo with the banner "login shell for marie" 
 * and the logged-in user credentials to the client socket.
 */
void send_ascii_logo(int socket_fd, const char *username, const char *password);

#endif