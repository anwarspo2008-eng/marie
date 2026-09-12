#include <string.h>
#include <sys/socket.h>
#include "shelltypes.h"

void about_shell(shell_ctx_t *ctx)
{
    const char *about =
        "\n"
        "name : Marie Server\n"
        "version : 0 . 1"
        "description : A lightweight, self-hosted socket server for a home lab.\n"
        "state : still under devlopement"
        "Type 'help' to see available shells.\n> ";
    send(ctx->socket_fd, about, strlen(about), 0);
}

