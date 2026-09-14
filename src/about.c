#include <string.h>
#include <sys/socket.h>
#include "shelltypes.h"

void about_shell(shell_ctx_t *ctx)
{
    const char *about =
        "\n"
        "name : Marie Server\n"
        "version : 0 . 1 \n"
        "description : A lightweight, self-hosted socket server for a home lab.\n"
        "state : still under devlopement \n"
        "Type 'help' to see available shells.\n> ";
        "MORE FEATURES SOON"
    send(ctx->socket_fd, about, strlen(about), 0);
}

