#include "headers.h"

char *trim(char *str)
{
    // Remove leading whitespace
    while (isspace((unsigned char)*str))
    {
        str++;
    }

    // Check for an empty string
    if (*str == '\0')
    {
        return str;
    }

    // Remove trailing whitespace
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
    {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    return str;
}

void function_execution(char *command, char *directory)
{
    // printf("%s\n",command);
    char *comm[4096];
    int count = 0;
    char *token = strtok(command, " \t\n");
    while (token != NULL)
    {
        comm[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    comm[count] = NULL;
    pid_t child_pid = fork();
    if (child_pid == -1)
    {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (child_pid == 0)
    {
        foreground_pid = child_pid;
        if (execvp(comm[0], comm) == -1)
        {
            perror("ERROR");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        int status;
        waitpid(child_pid, &status, 0);
    }
    // prompt(directory);
}

// void fgbg(char *command)
// {
//     pid_t pid, cid;

//     // Split the command into an array of arguments
//     char *token;
//     char *commands[100]; // Assuming a maximum of 100 arguments
//     int i = 0;
//     token = strtok(command, "&");
//     while (token != NULL)
//     {
//         commands[i] = token;
//         i++;
//         token = strtok(NULL, "&");
//     }
//     commands[i] = NULL;

//     // Create a child process
//     pid = fork();

//     if (pid < 0)
//     {
//         perror("Fork failed");
//         exit(1);
//     }
//     else if (pid == 0)
//     {
//         // Child process
//         if (bg == 1)
//         {
//             setpgid(0, 0); // Set the child process group ID to its own PID
//         }
//         execvp(commands[0], commands); // Execute the command
//         perror("Execvp failed");
//         exit(1);
//     }
//     else
//     {
//         // Parent process
//         if (bg == 1)
//         {
//             // Add the child process to a process list
//             // You can implement this part according to your needs
//             // Example: add_to_proclist(pid, command);
//         }
//         else
//         {
//             int status;
//             // Wait for the child to finish
//             waitpid(pid, &status, 0);
//         }
//     }
// }

// void fgbg(char* command)
// {
//     // Split the command into an array of arguments
//     char* token;
//     char* commands[100]; // Assuming a maximum of 100 arguments
//     int i = 0;
//     token = strtok(command, "&");
//     while (token != NULL)
//     {
//         commands[i] = token;
//         i++;
//         token = strtok(NULL, "&");
//     }
//     commands[i] = NULL;

//     // Create a child process
//     pid_t pid;

//     int is_bg = 0; // 0 for foreground, 1 for background
//     struct timeval start_time, end_time;
//     gettimeofday(&start_time, NULL);

//     for (int j = 0; j < i; j++) {
//         pid = fork();

//         if (pid < 0)
//         {
//             perror("Fork failed");
//             exit(1);
//         }
//         else if (pid == 0)
//         {
//             // Child process
//             if (is_bg == 1)
//             {
//                 setpgid(0, 0); // Set the child process group ID to its own PID
//             }

//             // Execute the command
//             char* single_command = strtok(commands[j], " ");
//             char* single_args[100]; // Assuming a maximum of 100 arguments
//             int k = 0;
//             //printf("%s\n",single_command);
//             while (single_command != NULL)
//             {
//                 single_args[k] = single_command;
//                 k++;
//                 single_command = strtok(NULL, " ");
//             }
//             single_args[k] = NULL;
//             //printf("%s\n",single_command);

//             execvp(single_args[0], single_args);
//             perror("Execvp failed");
//             exit(1);
//         }
//         else
//         {
//             // Parent process
//             if (is_bg == 1)
//             {
//                 // Print the PID of the newly created background process
//                 printf("%d\n", pid);
//             }
//             else
//             {
//                 gettimeofday(&end_time, NULL);
//                 int elapsed_time = (end_time.tv_sec - start_time.tv_sec);

//                 int status;
//                 // Wait for the child to finish
//                 waitpid(pid, &status, 0);

//                 // Print the time taken if > 2 seconds
//                 if (WIFEXITED(status)) {
//                     int exit_status = WEXITSTATUS(status);
//                     if (exit_status == 0) {
//                         // The command exited normally
//                         printf("Command exited normally\n");
//                     } else {
//                         // The command exited with an error
//                         printf("Command exited with an error\n");
//                     }
//                 } else {
//                     printf("Command terminated abnormally\n");
//                 }

//                 if (elapsed_time > 2) {
//                     printf("Time taken by the process: %d seconds\n", elapsed_time);
//                     printf("Process name: %s\n", commands[j]);
//                 }

//             }
//         }

//         // Toggle is_bg to switch between foreground and background
//         is_bg = 1 - is_bg;
//     }
// }

void fgbg(char *command)
{
    // Split the command into an array of arguments
    char *token;
    char *commands[100]; // Assuming a maximum of 100 arguments
    int i = 0;
    token = strtok(command, "&");
    while (token != NULL)
    {
        commands[i] = trim(token); // Trim leading and trailing whitespace
        i++;
        token = strtok(NULL, "&");
    }
    commands[i] = NULL;

    // Create a child process
    pid_t pid;
    int is_bg = 1; // 0 for foreground, 1 for background

    for (int j = 0; j < i; j++)
    {
        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        pid = fork();

        if (pid < 0)
        {
            perror("Fork failed");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            if (is_bg == 1)
            {
                setpgid(0, 0); // Set the child process group ID to its own PID
                background_pid = pid;
            }

            // Execute the command
            char *single_command = strtok(commands[j], " ");
            char *single_args[100]; // Assuming a maximum of 100 arguments
            int k = 0;
            while (single_command != NULL)
            {
                single_args[k] = single_command;
                k++;
                single_command = strtok(NULL, " ");
            }
            single_args[k] = NULL;

            execvp(single_args[0], single_args);
            perror("Execvp failed");
            exit(1);
        }
        else
        {
            // Parent process
            if (is_bg == 1)
            {
                // Print the PID of the newly created background process
                printf("%d\n", pid);
                //addBackgroundProcess(pid, commands[j], 0);
                gettimeofday(&end_time, NULL);
                int elapsed_time = (end_time.tv_sec - start_time.tv_sec);
                printf("..%d..\n",elapsed_time);

                // Print the time taken if > 2 seconds
                if (elapsed_time > 2)
                {
                    printf("Time taken by the process: %d seconds\n", elapsed_time);
                    printf("Process name: %s\n", commands[j]);
                }
            }
            else
            {
                int status;
                // Wait for the child to finish
                foreground_pid = pid;
                waitpid(pid, &status, 0);
            }
        }

        // Toggle is_bg to switch between foreground and background
        is_bg = 1 - is_bg;
    }
}
