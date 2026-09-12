#include <string.h>
#include <stdio.h>
#include <sys/socket>

static const char *shellmodes = 
    {
        loginshell ,
        help ,
        about
    }
void typeshell(int client_fd , const char *input_mode , const char *username)
{
    char res[512];
    int mode_found = 0;
    int i = 0;

    for (i = 0 ;shellmodes[i] != '\0' ; i ++)
    {
        if (strcmp(input_mode , shellmodes[i]) == 0)
        {
            mode_found = 1;
               if (strcmp(input_mode , help) == 0){
                
               } 
        }
    }
    if (!mode found)
    {
        snprintf(res , sizeof(res),"Unknown command try : help \n" , input_mode);
        send(client_fd , res , strlen(res), 0);
    }
    if (strcmp())

}
    