#include "headers.h"

BackgroundProcess backgroundProcesses[MAX_BACKGROUND_PROCESSES]; // Define an array to store background processes
int numBackgroundProcesses = 0;                                  // Keep track of the number of background processes

// Function to add a background process to the data structure
void addBackgroundProcess(pid_t pid, const char *command, int status)
{
    if (numBackgroundProcesses < MAX_BACKGROUND_PROCESSES)
    {
        backgroundProcesses[numBackgroundProcesses].pid = pid;
        strncpy(backgroundProcesses[numBackgroundProcesses].command, command, sizeof(backgroundProcesses[numBackgroundProcesses].command));
        backgroundProcesses[numBackgroundProcesses].status = status;
        numBackgroundProcesses++;
    }
    else
    {
        printf("Maximum number of background processes reached.\n");
    }
}

// Function to find the index of a background process by its pid
int findBackgroundProcessIndex(pid_t pid)
{
    for (int i = 0; i < numBackgroundProcesses; i++)
    {
        if (backgroundProcesses[i].pid == pid)
        {
            return i;
        }
    }
    return -1; // Process not found
}

// Function to bring a background process to foreground (fg)
// void bringToForeground(pid_t pid)
// {
//     printf("%d\n", pid);
//     if (kill(pid, SIGCONT) == -1)
//     {
//         perror("kill");
//         fprintf(stderr, "No such process found with PID %d\n", pid);
//         return;
//     }

//     // Wait for the process to finish
//     int status;
//     if (waitpid(pid, &status, 0) == -1)
//     {
//         perror("waitpid");
//         return;
//     }

//     if (WIFEXITED(status))
//     {
//         printf("Process with PID %d exited with status %d\n", pid, WEXITSTATUS(status));
//     }
//     else if (WIFSIGNALED(status))
//     {
//         printf("Process with PID %d terminated by signal %d\n", pid, WTERMSIG(status));
//     }
//     else
//     {
//         printf("Process with PID %d finished\n", pid);
//     }
// }

void bringToForeground(pid_t pid)
{
    tcsetpgrp(STDIN_FILENO, pid);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    kill(pid, SIGCONT);
    int status;
    waitpid(-1, &status, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}

// Function to change the state of a stopped background process to running (bg)
void runInBackground(pid_t pid)
{
    if (kill(pid, SIGCONT) == -1)
    {
        printf("Error: No such process found\n");
        return;
    }
}