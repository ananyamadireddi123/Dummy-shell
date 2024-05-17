#include "headers.h"

void proclore(char *command)
{
    char *args = strtok(NULL, " "); // Get the argument after "proclore"

    if (args == NULL)
    {
        // Print information about the shell process
        printf("pid : %d\n", getpid());
        printf("process status : R+\n"); // Assuming the shell is always considered foreground
        printf("Process Group : %d\n", getpgid(0));
        // printf("Virtual memory : N/A\n");
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0)
        {
            printf("Virtual memory : %ld KB\n", usage.ru_maxrss);
        }
        else
        {
            perror("Error getting resource usage");
        }
        // printf("executable path : N/A\n");

        char exe_path[4096];
        snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", getpid());

        char buf[4096];
        ssize_t len = readlink(exe_path, buf, sizeof(buf) - 1);

        if (len != -1)
        {
            buf[len] = '\0';
            printf("executable path : %s\n", buf);
        }
        else
        {
            perror("Error getting executable path");
        }
    }
    else
    {
        // Print information about the specified process
        int pid = atoi(args);
        printf("pid : %d\n", pid);
        printf("process status : R\n"); // Assuming all specified processes are running
        printf("Process Group : %d\n", getpgid(pid));
        // Virtual memory and executable path information might require further system calls.
        // printf("Virtual memory : N/A\n");
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0)
        {
            printf("Virtual memory : %ld KB\n", usage.ru_maxrss);
        }
        else
        {
            perror("Error getting resource usage");
        }
        // printf("executable path : N/A\n");
        char exe_path[4096];
        snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", pid);

        char buf[4096];
        ssize_t len = readlink(exe_path, buf, sizeof(buf) - 1);

        if (len != -1)
        {
            buf[len] = '\0';
            printf("executable path : %s\n", buf);
        }
        else
        {
            perror("Error getting executable path");
        }
    }
}