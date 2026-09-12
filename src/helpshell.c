// standard C libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
// Custom project header
#include "shelltypes.h"

void help_shell(shell_ctx_t *ctx)
{
    // help message in ENG
    const char *help_msg_eng =
        "\n \033[1;36m=========================== [MARIE HELP SHELL] =========================\033[0m\n"
        " \033[1;32mAvailable commands:\033[0m\n\n"
        " \033[1;33m  help        \033[0m- Show the available commands in this interaction window.\n"
        " \033[1;33m  loginshell  \033[0m- Show the user login interaction menu.\n"
        " \033[1;33m  about       \033[0m- Show information about the project state and commands.\n"
        "\n \033[1;36m========================================================================\033[0m\n> ";

    // help message FR
    const char *help_msg_fr =
        "\n \033[1;36m========================== [MARIE HELP SHELL] ==========================\033[0m\n"
        " \033[1;32mCommandes disponibles:\033[0m\n\n"
        " \033[1;33m  help        \033[0m- Afficher les commandes disponibles dans cette fenêtre.\n"
        " \033[1;33m  loginshell  \033[0m- Afficher le menu d'interaction de connexion utilisateur.\n"
        " \033[1;33m  about       \033[0m- Afficher les informations sur l'état du projet.\n"
        "\n \033[1;36m========================================================================\033[0m\n> ";

    // help message AR
    const char *help_msg_ar =
        "\n \033[1;36m========================== [MARIE HELP SHELL] ==========================\033[0m\n"
        " \033[1;32mالأوامر المتاحة:\033[0m\n\n"
        "   عرض الأوامر المتاحة في نافذة التفاعل هذه - \033[1;33mhelp       \033[0m\n"
        "       عرض قائمة التفاعل الخاصة بتسجيل الدخول - \033[1;33mloginshell \033[0m\n"
        "                 عرض معلومات حول حالة المشروع - \033[1;33mabout      \033[0m\n"
        "\n \033[1;36m========================================================================\033[0m\n> ";

    const char *lang_menu =
        " __________________________________________ \n"
        "|                                          |\n"
        "|            LANGUAGE_MODES                |\n"
        "|__________________________________________|\n"
        "|                                          |\n"
        "|   [1] ENG                                |\n"
        "|   [2] FR                                 |\n"
        "|   [3] AR                                 |\n"
        "|__________________________________________|\n"
        "| >>> Select an option (default eng): ";

    // Everything meant for the client has to go over the socket, not
    // printf/scanf: this code runs in a forked child talking to a
    // remote client, so the server's own stdin/stdout aren't connected
    // to the person on the other end of client_fd.
    send(ctx->socket_fd, lang_menu, strlen(lang_menu), 0);

    char choice_buf[16] = {0};
    ssize_t bytes = recv(ctx->socket_fd, choice_buf, sizeof(choice_buf) - 1, 0);
    if (bytes <= 0) {
        return; // client disconnected mid-menu
    }
    choice_buf[bytes] = '\0';

    int choice = atoi(choice_buf); // returns 0 on empty/invalid input -> falls to default

    switch (choice) {
        case 1:
            send(ctx->socket_fd, help_msg_eng, strlen(help_msg_eng), 0);
            break;
        case 2:
            send(ctx->socket_fd, help_msg_fr, strlen(help_msg_fr), 0);
            break;
        case 3:
            send(ctx->socket_fd, help_msg_ar, strlen(help_msg_ar), 0);
            break;
        default:
            send(ctx->socket_fd, help_msg_eng, strlen(help_msg_eng), 0);
            break;
    }
}
