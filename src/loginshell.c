#include <string.h>
#include <sys/socket.h>
#include "shelltypes.h"
#include "auth.h"
#include "Look.h"

// Removes trailing '\n'/'\r' from client input (e.g. from netcat/telnet).
static void trim_line(char *str)
{
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

void loginshell_shell(shell_ctx_t *ctx)
{
    char buf[128];

    const char *u_prompt = "Username: ";
    send(ctx->socket_fd, u_prompt, strlen(u_prompt), 0);

    memset(buf, 0, sizeof(buf));
    ssize_t u_bytes = recv(ctx->socket_fd, buf, sizeof(buf) - 1, 0);
    if (u_bytes <= 0) return;
    buf[u_bytes] = '\0';
    trim_line(buf);
    strncpy(ctx->username, buf, 127);
    ctx->username[127] = '\0';

    const char *p_prompt = "Password: ";
    send(ctx->socket_fd, p_prompt, strlen(p_prompt), 0);

    memset(buf, 0, sizeof(buf));
    ssize_t p_bytes = recv(ctx->socket_fd, buf, sizeof(buf) - 1, 0);
    if (p_bytes <= 0) return;
    buf[p_bytes] = '\0';
    trim_line(buf);
    strncpy(ctx->password, buf, 127);
    ctx->password[127] = '\0';

    if (check_credentials(ctx->username, ctx->password)) {
        *ctx->authenticated = 1;
        const char *ok_msg = "Login successful!\n\n";
        send(ctx->socket_fd, ok_msg, strlen(ok_msg), 0);

        // Send ASCII banner with the username (password is no longer
        // echoed back to the screen -- see Look.c/Look.h).
        send_ascii_logo(ctx->socket_fd, ctx->username , ctx->password);
    } else {
        const char *err_msg = "Invalid username or password. Type 'loginshell' to try again.\n> ";
        send(ctx->socket_fd, err_msg, strlen(err_msg), 0);
    }
}
