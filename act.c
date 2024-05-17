#include "headers.h"




void listProcesses() {
    DIR *dir;
    struct dirent *entry;
    char proc_path[256];

    // Open the /proc directory
    dir = opendir("/proc");

    if (dir == NULL) {
        perror("Failed to open /proc directory");
        return;
    }

    printf("PID : Command Name - State\n");

    while ((entry = readdir(dir)) != NULL) {
        if (atoi(entry->d_name) > 0) {
            // Construct the full path for the /proc/<pid>/cmdline file
            snprintf(proc_path, sizeof(proc_path), "/proc/%s/cmdline", entry->d_name);

            // Open the cmdline file for reading
            FILE *cmdline_file = fopen(proc_path, "r");
            if (cmdline_file == NULL) {
                perror("Failed to open cmdline file");
                continue;
            }

            // Read the command name from the cmdline file
            char command_name[256];
            size_t len = fread(command_name, 1, sizeof(command_name) - 1, cmdline_file);
            fclose(cmdline_file);
            command_name[len] = '\0'; // Null-terminate the string

            // Check if the process is running or stopped
            snprintf(proc_path, sizeof(proc_path), "/proc/%s/status", entry->d_name);
            FILE *status_file = fopen(proc_path, "r");
            if (status_file == NULL) {
                perror("Failed to open status file");
                continue;
            }

            char line[256];
            char state[16] = "unknown"; // Default state

            // Read the status file line by line to find the state
            while (fgets(line, sizeof(line), status_file)) {
                if (strncmp(line, "State:", 6) == 0) {
                    // Extract the state from the "State:" line
                    char *state_ptr = strchr(line, ':');
                    if (state_ptr != NULL) {
                        strcpy(state, state_ptr + 2);
                        break;
                    }
                }
            }
            fclose(status_file);

            // Remove trailing newline characters from the command name and state
            command_name[strcspn(command_name, "\n")] = '\0';
            state[strcspn(state, "\n")] = '\0';

            // Print the PID, command name, and state
            printf("%s : %s - %s\n", entry->d_name, command_name, state);
        }
    }

    closedir(dir);
}

 