#include "headers.h"

void out_direct(char *output_file, char *token, char *command, char *directory )
{
    if (output_file != NULL)
    {
        // Remove leading and trailing spaces from output_file
        output_file = trim(output_file);
        int fd;
        if (strstr(token, ">>"))
        {
            // Append mode
            // printf("error 89");
            fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            // Overwrite mode
            // printf("error 94");
            fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (fd == -1)
        {
            perror("Error opening output file");
        }
        else
        {
            // printf("%s",command);
            int temp = dup(STDOUT_FILENO);
            dup2(fd, STDOUT_FILENO);
            function_execution(command, directory );
            close(fd);
            dup2(temp, STDOUT_FILENO);
        }
    }
    else
    {
        // Handle invalid syntax
        printf("No output file given\n");
    }
}

void in_direct(char *input_file, char *token, char *command, char *directory )
{
    if (input_file != NULL)
    {
        // Remove leading and trailing spaces from input_file
        input_file = trim(input_file);
        int fd = open(input_file, O_RDONLY);
        if (fd == -1)
        {
            perror("No such input file found");
        }
        else
        {
            int temp = dup(STDIN_FILENO);
            dup2(fd, STDIN_FILENO);
            function_execution(command, directory );
            close(fd);
            dup2(temp, STDIN_FILENO);
        }
    }
    else
    {
        // Handle invalid syntax
        printf("No input file given\n");
    }
}

void in_direct1(char *input_file, char *token, char *command, char *directory, int pipe_fd[2])
{
    if (input_file != NULL)
    {
        // Remove leading and trailing spaces from input_file
        input_file = trim(input_file);
        int fd = open(input_file, O_RDONLY);
        if (fd == -1)
        {
            perror("No such input file found");
        }
        else
        {
            int temp = dup(STDIN_FILENO);
            dup2(fd, STDIN_FILENO);
            function_execution(command, directory);
            close(fd);
            dup2(temp, STDIN_FILENO);
        }
    }
    else
    {
        // Handle invalid syntax
        printf("No input file given\n");
    }
}
