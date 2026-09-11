#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "Look.h"

void send_ascii_logo(int socket_fd, const char *username, const char *password)
{
    char logo[2048];

    snprintf(logo, sizeof(logo),
        "\033[2J\033[H"
        "=========================================\n"
        "          login shell for marie          \n"
        "=========================================\n\n"
        "⠀⣾⠙⢶⠦⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⠀⣏⠱⣌⠣⡈⠓⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⢾⢺⣤⡘⢦⡈⢢⡀⠱⣤⢶⠀⠀⠀⠀⠀⠀⠀⠀⠀\n"
        "⢸⡿⡏⠻⡀⠳⡄⠹⡄⠹⡸⡇⠀⠀⠀⠀⠀⠀⠀⠀   [LOGGED IN USER]\n"
        "⣸⠃⢳⡀⢳⠀⢹⡀⠱⡄⠳⣹⣄⠀⠀⠀⠀⠀⠀⠀   Username: %s\n"
        "⢿⠀⢮⢇⠈⠣⠀⠓⠀⠈⠀⠀⠹⡆⠀⠀⠀⠀⠀⠀   Password: %s\n"
        "⠈⢧⠈⢿⣆⠀⠀⠀⠀⠀⠀⠀⠀⣏⠀⠀⠀⠀⠀⠀\n"
        "⠀⠀⡇⠈⢹⡄⠀⠀⠀⠀⠀⠀⠀⠹⡀⠀⠀⠀⠀⠀\n"
        "⠀⠀⢳⠀⠈⢧⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠀⠀\n"
        "⠀⠀⠈⢧⠀⠈⣆⡀⠀⠀⠀⠀⠀⠀⢻⣤⣤⣤⣀⡀\n"
        "⠀⠀⢀⣾⣦⠀⠈⠳⣄⠀⠀⠀⠀⢠⣾⣿⣿⣿⡟⠁\n"
        "⠀⠀⣿⣿⣿⠀⠀⠀⠈⣳⣤⡀⣰⣿⣿⣿⣿⡟⠀⠀\n"
        "⠀⠀⣿⣿⣿⣀⣀⣀⣴⣿⡗⠳⣿⣿⣿⣿⣿⠃⠀⠀\n"
        "⠀⠀⠹⣿⣿⣿⣿⣿⣿⡿⠁⢰⣿⣿⣿⣿⡏⠀⠀⠀\n"
        "⠀⠀⠀⠙⠻⠿⠿⠿⠛⠁⠀⠈⠛⠿⣿⣿⠁⠀⠀⠀\n\n",
        username, password
    );

    send(socket_fd, logo, strlen(logo), 0);
}