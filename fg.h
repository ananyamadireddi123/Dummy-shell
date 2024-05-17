#ifndef SHIFT
#define SHIFT
#include "headers.h"

#define MAX_BACKGROUND_PROCESSES 100
typedef struct {
    pid_t pid;
    char command[4096]; // Store the command associated with the process
    int status; // 0 for Stopped, 1 for Running
} BackgroundProcess;

void addBackgroundProcess(pid_t pid, const char *command, int status);
int findBackgroundProcessIndex(pid_t pid);
void bringToForeground(pid_t pid);
void runInBackground(pid_t pid);



#endif