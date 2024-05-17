#include "headers.h"

void ping(int pid, int signal_number)
{
    // Check if the process exists
    if (kill(pid, 0) == -1)
    {
        perror("No such process found");
    }
    else
    {
        // Calculate the actual signal number based on modulo 32
        int actual_signal = signal_number % 32;

        // Send the signal to the process
        if (kill(pid, actual_signal) == -1)
        {
            perror("Failed to send signal");
        }
        else
        {
            printf("Sent signal %d to process with pid %d\n", actual_signal, pid);
        }
    }
}

 