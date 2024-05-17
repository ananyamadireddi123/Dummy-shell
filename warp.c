#include "headers.h"

char lastbutone[4096]={'\0'};

void warp_execution(char *args, char *directory, char *previous_directory)
{
    char *home_dir = getenv("HOME");
    int avail=0;
    char new_directory[4096];
     
     
    if (args == NULL)
    {
        chdir(directory);
    }
    else if (strcmp(args, "~") == 0)
    {
        // getcwd(home_dir, 4096);
        chdir(directory);
    }
    else if (strcmp(args, "-") == 0)
    {
        // char temp[4096];
        // strcpy(temp, directory);
        // strcpy(directory, previous_directory);
        // strcpy(previous_directory, temp);
        if (lastbutone[0]!='\0')
        {
             
            chdir(lastbutone);
        }
        else
        {
            printf("owd not set\n");
            avail=1;
        }
    }
    else if (strcmp(args, ".") == 0)
    {
    }
    else if (strcmp(args, "..") == 0)
    {
        getcwd(lastbutone,sizeof(lastbutone));
        chdir("..");
    }
    else
    {
        if (args[0] == '~')
        {
            snprintf(new_directory, sizeof(new_directory), "%s", home_dir);
            strcat(new_directory, args + 1);
        }
        else
        {
            snprintf(new_directory, sizeof(new_directory), "%s", args);
        }
        getcwd(lastbutone,sizeof(lastbutone));
        chdir(new_directory);
    }

    // Print the full path of the new working directory
    char current_directory[4096];
    getcwd(current_directory, sizeof(current_directory));
    if(avail==0)
    {
    printf("%s\n", current_directory);
    }
    // printf(".....%s.....\n",previous_directory);
    
    if(strcmp(current_directory,directory)!=0 && lastbutone[0]=='\0')
    {
        //lastbutone=(char*)malloc(sizeof(char)*4096);
        //printf("..%s..\n",directory);
        strcpy(lastbutone,directory);
         
    }
         
    strncpy(directory, current_directory, sizeof(directory));
}
