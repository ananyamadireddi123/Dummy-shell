#include "headers.h"

void prompt(char *directory, char *command)
{
    // Do not hardcode the prmopt
    // printf("<Everything is a file> ");

    char *username = getlogin();
    if (username == NULL)
    {
        perror("getlogin");
        exit(EXIT_FAILURE);
    }

    struct utsname system_name;
    if (uname(&system_name) != 0)
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }

    // printf("<%s@%s:~>",username,system_name.nodename);

    char prompt_path[4096];
    char current[4096];

    getcwd(current, 4096);
    if (strlen(directory) > strlen(current))
    {
        strcpy(prompt_path, current);
    }
    else
    {
        strcpy(prompt_path, current + strlen(directory));
    }
    if (command == NULL)
    {
        printf("<%s@%s:%s>", username, system_name.nodename, prompt_path);
    }
    else
    {
        printf("<%s@%s:%s:%s>", username, system_name.nodename, prompt_path,command);
    }
}
