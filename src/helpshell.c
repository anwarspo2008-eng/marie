//standard C libraries
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
// Custom project header
#include "shelltypes.h"
void help(int client_fd)
{
    int choice;

    //help message in ENG

const char *help_msg_eng = 
    "\n \033[1;36m=========================== [MARIE HELP SHELL] =========================\033[0m\n"
    " \033[1;32mAvailable commands:\033[0m\n\n"
    " \033[1;33m  HELP        \033[0m- Show the available commands in this interaction window.\n"
    " \033[1;33m  loginshell  \033[0m- Show the user login interaction menu.\n"
    " \033[1;33m  about       \033[0m- Show information about the project state and commands.\n"
    "\n \033[1;36m========================================================================\033[0m\n";
   
    //help message FR

const char *help_msg_fr = 
    "\n \033[1;36m========================== [MARIE HELP SHELL] ==========================\033[0m\n"
    " \033[1;32mCommandes disponibles:\033[0m\n\n"
    " \033[1;33m  HELP        \033[0m- Afficher les commandes disponibles dans cette fenêtre.\n"
    " \033[1;33m  loginshell  \033[0m- Afficher le menu d'interaction de connexion utilisateur.\n"
    " \033[1;33m  about       \033[0m- Afficher les informations sur l'état du projet.\n"
    "\n \033[1;36m========================================================================\033[0m\n";
    
    //help message AR
    
const char *help_msg_ar = 
    "\n \033[1;36m========================== [MARIE HELP SHELL] ==========================\033[0m\n"
    " \033[1;32mالأوامر المتاحة:\033[0m\n\n"
    "   عرض الأوامر المتاحة في نافذة التفاعل هذه - \033[1;33mHELP       \033[0m\n"
    "       عرض قائمة التفاعل الخاصة بتسجيل الدخول - \033[1;33mloginshell \033[0m\n"
    "                 عرض معلومات حول حالة المشروع - \033[1;33mabout      \033[0m\n"
    "\n \033[1;36m========================================================================\033[0m\n";
printf(" __________________________________________ \n");
printf("|                                          |\n");
printf("|            LAUNGUAGE_MODES               |\n");
printf("|__________________________________________|\n");
printf("|                                          |\n");
printf("|   [1] ENG                                |\n");
printf("|   [2] fr                                 |\n");
printf("|   [3] ar                                 |\n");
printf("|__________________________________________|\n");
printf("| >>> Select an option (default eng):");
    scanf('%d', choice);
    switch (choice) 
    {
        case 1:
            send(client_fd,help_msg_eng,strlen(help_msg_eng));
            break;

        case value2:
            send(client_fd,help_msg_fr,strlen(help_msg_fr));
            break;

        case value3:
            send(client_fd,help_msg_ar,strlen(help_msg_ar));
            break;

        default:
            send(client_fd,help_msg_eng,strlen(help_msg_eng));
            break;
    }

}
