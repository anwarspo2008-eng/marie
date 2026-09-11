#ifndef SERVER_H
#define SERVER_H

// Starts the server: creates the socket, binds, listens, and
// runs the accept loop. Blocks forever (or until the server
// is shut down).
void start_server(void);

#endif // SERVER_H