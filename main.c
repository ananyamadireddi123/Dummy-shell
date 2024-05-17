#include "headers.h"

// Function to execute a command with pipes
pid_t foreground_pid = 0;
pid_t background_pid = 0;

volatile sig_atomic_t running = 1;

// Signal handler for Ctrl-C (SIGINT)
void handle_sigint(int sig)
{
    if (foreground_pid != 0)
    {
        // Send SIGINT to the foreground process
        kill(foreground_pid, SIGINT);
    }
}

// Signal handler for Ctrl-Z (SIGTSTP)
void handle_ctrl_z(int signum)
{
    foreground_pid = getpid();
    if (foreground_pid != 0)
    {
        kill(foreground_pid, SIGTSTP);
        char directory[4096];
        getcwd(directory, 4096);
        prompt(directory, NULL);
        return;
    }
}

// Signal handler for Ctrl-D (EOF)
int handle_sigeof(int sig)
{
    int pid = getpid();
    int result = kill(pid, SIGKILL);
    exit(0);
}

char previous_directory[4096] = {'\0'};

int main()
{
    // Keep accepting commands
    char directory[4096];
    getcwd(directory, 4096);
    strcpy(previous_directory, directory);
    int avail = 0;
    History history;
    history.count = 0;
    loadHistory(&history);

    signal(SIGINT, handle_sigint);
    signal(SIGTSTP, handle_ctrl_z);

    signal(SIGTERM, handle_sigeof);

    while (1)
    {
        prompt(directory, NULL);
        char input[4096];
        if (fgets(input, 4096, stdin) == NULL)
        {
            // Ctrl-D (EOF) was pressed, exit gracefully
            printf("\n");
            break;
        }
        // Print appropriate prompt with username, systemname and directory before accepting input

        int avail = 0;
        int background_process_count = 0;
        int counter = 0;
        int fgbgcheck = 0;

        input[strcspn(input, "\n")] = '\0';

        int in_redirection = 0;   // Flag to indicate input redirection
        int out_redirection = 0;  // Flag to indicate output redirection
        char *input_file = NULL;  // Name of the input file
        char *output_file = NULL; // Name of the output file
        int pipe_count = 0;       // Count of pipes
        int pipe_fds[2];
        char *left_command = NULL; // Command on the left side of the pipe
        char *last_command = NULL; // Command on the right side of the last pipe

        char *saveptr1;
        char *token1 = strtok_r(input, ";", &saveptr1);
        // printf("////%s////\n",token);
        while (token1 != NULL)
        {
            // printf("%s\n", token1);
            char *string = (char *)malloc(sizeof(char) * MAX_BUFFER_SIZE);
            strcpy(string, token1);
            int ptr = 0;
            char *saveptr;
            if (strchr(token1, '&') != NULL)
            {
                counter = 1;
            }
            char *token = strtok_r(token1, "&", &saveptr);

            while (token != NULL)
            {
                char temp[4096];
                strcpy(temp, token);
                char *command = strtok(temp, " ");

                // Inside the main loop, after parsing the user input
                if (strstr(token, "|"))
                {
                    // Initialize variables to track pipe and redirection status
                    int num_pipes = 0;
                    int in_redirection = 0;
                    int out_redirection = 0;
                    char *input_file = NULL;
                    char *output_file = NULL;

                    // Split the input by pipes
                    char *pipe_commands[MAX_COMMANDS];
                    char *token_pipe = strtok(token, "|");
                    while (token_pipe != NULL && num_pipes < MAX_COMMANDS)
                    {
                        pipe_commands[num_pipes] = strdup(token_pipe);
                        num_pipes++;
                        token_pipe = strtok(NULL, "|");
                    }

                    // Create pipes for communication
                    int pipes[MAX_COMMANDS - 1][2];
                    for (int i = 0; i < num_pipes - 1; i++)
                    {
                        if (pipe(pipes[i]) == -1)
                        {
                            perror("Pipe creation failed");
                            exit(EXIT_FAILURE);
                        }
                    }

                    // Process each pipe command
                    // Inside the loop where you process each pipe command
                    for (int i = 0; i < num_pipes; i++)
                    {
                        // Reset redirection flags and file names
                        in_redirection = 0;
                        out_redirection = 0;
                        input_file = NULL;
                        output_file = NULL;
                        int tester = 0;

                        // Check for input and output redirection
                        if (strstr(pipe_commands[i], ">"))
                        {
                            // Output redirection
                            out_redirection = 1;
                            char *output_token = strtok(pipe_commands[i], ">");
                            if (output_token != NULL)
                            {
                                output_file = trim(strtok(NULL, ">"));
                            }
                        }
                        else if (strstr(pipe_commands[i], "<"))
                        {
                            // Input redirection
                            in_redirection = 1;
                            char *input_token = strtok(pipe_commands[i], "<");
                            if (input_token != NULL)
                            {
                                input_file = trim(strtok(NULL, "<"));
                            }
                        }

                        // Split the command by spaces
                        char *command1 = strtok(pipe_commands[i], " ");

                        // Create a child process to execute the command
                        pid_t child_pid = fork();
                        if (child_pid == -1)
                        {
                            perror("Fork failed");
                            exit(EXIT_FAILURE);
                        }
                        else if (child_pid == 0)
                        {
                            // Child process

                            // Handle input redirection

                            if (in_redirection)
                            {
                                int pipe_fd[2];
                                pid_t child_pid;

                                // Create a pipe
                                if (pipe(pipe_fd) == -1)
                                {
                                    perror("pipe");
                                    exit(EXIT_FAILURE);
                                }

                                // Fork a child process
                                if ((child_pid = fork()) == -1)
                                {
                                    perror("fork");
                                    exit(EXIT_FAILURE);
                                }

                                if (child_pid == 0)
                                {
                                    // Child process
                                    close(pipe_fd[0]);               // Close the read end of the pipe
                                    dup2(pipe_fd[1], STDOUT_FILENO); // Redirect stdout to the pipe
                                    close(pipe_fd[1]);               // Close the write end of the pipe

                                    // Execute 'cat' command with input from input.txt
                                    //printf("%s\n", command);
                                    in_direct(input_file, token, command, directory);

                                    // Parent process
                                                  // Close the read end of the pipe

                                    // Execute 'wc' command to count words, lines, and characters
                                    char *args[MAX_ARGS]; // Define MAX_ARGS appropriately
                                    int arg_count = 0;

                                    // Add the command and its arguments to args
                                    args[arg_count++] = command1;

                                    // Tokenize the rest of the command and add its arguments to args
                                    char *token_arg = strtok(NULL, " ");
                                    while (token_arg != NULL)
                                    {
                                        args[arg_count++] = token_arg;
                                        token_arg = strtok(NULL, " ");
                                    }

                                    args[arg_count] = NULL; // Null-terminate the args array
                                    printf("%s\n", args[1]);
                                    function_execution(args[1], directory);
                                }
                                else
                                {
                                    wait(NULL);
                                    close(pipe_fd[1]);              // Close the write end of the pipe
                                    dup2(pipe_fd[0], STDIN_FILENO); // Redirect stdin to the pipe
                                    close(pipe_fd[0]);
                                    
                                }

                                // Close unused pipe ends
                            }

                            // Handle output redirection
                            if (out_redirection)
                            {
                                out_direct(output_file, pipe_commands[i], command1, directory);
                            }

                            // Close unused pipe ends
                            if (i > 0)
                            {
                                close(pipes[i - 1][1]);
                                dup2(pipes[i - 1][0], STDIN_FILENO);
                                close(pipes[i - 1][0]);
                            }
                            if (i < num_pipes - 1)
                            {
                                close(pipes[i][0]);
                                dup2(pipes[i][1], STDOUT_FILENO);
                                close(pipes[i][1]);
                            }

                            // Execute the command
                            if (tester != 1)
                            {
                                function_execution(command1, directory);
                            }
                            else
                            {
                                tester = 0;
                            }

                            exit(EXIT_SUCCESS);
                        }

                        else
                        {
                            // Parent process

                            // Close unused pipe ends
                            if (i > 0)
                            {
                                close(pipes[i - 1][0]);
                                close(pipes[i - 1][1]);
                            }
                            waitpid(child_pid, NULL, 0);
                        }
                    }

                    // Wait for all child processes to finish
                    for (int i = 0; i < num_pipes; i++)
                    {
                        wait(NULL);
                    }

                    // Clean up allocated memory for pipe commands
                    for (int i = 0; i < num_pipes; i++)
                    {
                        free(pipe_commands[i]);
                    }
                }
                else if (strstr(token, ">"))
                {
                    // Output redirection
                    char *output_file = strtok(NULL, ">");
                    out_direct(output_file, token, command, directory);
                }

                else if (strstr(token, "<"))
                {
                    // Input redirection
                    char *input_file = strtok(NULL, "<");
                    in_direct(input_file, token, command, directory);
                }
                else
                {
                    if (strcmp(command, "warp") == 0)
                    {
                        char *args = strtok(NULL, " ");

                        warp_execution(args, directory, previous_directory);
                        avail = 1;
                    }

                    else if (strcmp(command, "pastevents") == 0)
                    {
                        char *args = strtok(NULL, "");
                        if (args == NULL)
                        {
                            for (int i = 0; i < history.count; i++)
                            {
                                printf("%s\n", history.commands[i]);
                            }
                        }
                        else if (strcmp(args, "purge") == 0)
                        {
                            history.count = 0;
                            saveHistory(&history);
                        }
                        else if (strstr(args, "execute") == args)
                        {
                            int index = atoi(args + strlen("execute"));
                            if (index >= 1 && index <= history.count)
                            {
                                executeCommand(history.commands[index - 1]);
                            }
                        }
                        else
                        {
                            if (strcmp(args, command) != 0 && strcmp(args, "pastevents") != 0)
                            {
                                executeCommand(args);
                            }
                            addCommandToHistory(&history, args);
                        }
                    }

                    else if (strcmp(command, "peek") == 0)
                    {
                        char *args = strtok(NULL, "");

                        int showHidden = 0;
                        int showDetails = 0;

                        char *flag = strtok(args, " ");
                        while (flag != NULL)
                        {
                            if (strcmp(flag, "-a") == 0)
                            {
                                showHidden = 1;
                            }
                            else if (strcmp(flag, "-l") == 0)
                            {
                                showDetails = 1;
                            }
                            else if (strcmp(flag, "-al") == 0 || strcmp(flag, "-la") == 0)
                            {
                                showHidden = 1;
                                showDetails = 1;
                            }
                            else
                            {
                                // If not a valid flag, treat it as part of the path
                                if (args == NULL)
                                {
                                    args = flag;
                                }
                                else
                                {
                                    // Concatenate the argument with the previous ones
                                    char *temp = args;
                                    args = (char *)malloc(strlen(temp) + strlen(flag) + 2);
                                    strcpy(args, temp);
                                    strcat(args, " ");
                                    strcat(args, flag);
                                    free(temp);
                                }
                            }

                            flag = strtok(NULL, " ");
                        }

                        char path[4096];
                        if (args[0] == '~')
                        {
                            snprintf(path, sizeof(path), "%s%s", getenv("HOME"), args + 1);
                        }
                        else if (args[0] == '.')
                        {
                            if (args[1] == '/')
                            {
                                snprintf(path, sizeof(path), "%s%s", directory, args + 1);
                            }
                            else
                            {
                                snprintf(path, sizeof(path), "%s/%s", directory, args + 1);
                            }
                        }
                        else if (args[0] == '-')
                        {
                            strcpy(path, previous_directory);
                        }
                        else
                        {
                            strcpy(path, args);
                        }
                        printf("%d %d\n", showHidden, showDetails);
                        listFilesAndDirectories(path, showHidden, showDetails);
                    }

                    else if (strcmp(command, "seek") == 0)
                    {
                        int onlyFiles = 0, onlyDirs = 0, executeFlag = 0;
                        char *search = NULL;
                        char *target_directory = NULL;
                        char *flags = strtok(token, " ");
                        flags = strtok(NULL, " ");
                        while (flags != NULL)
                        {
                            if (strcmp(flags, "-d") == 0)
                                onlyDirs = 1;
                            else if (strcmp(flags, "-f") == 0)
                                onlyFiles = 1;
                            else if (strcmp(flags, "-e") == 0)
                            {
                                executeFlag = 1;
                            }
                            else if (search == NULL)
                            {
                                search = (char *)malloc(sizeof(char) * 256);
                                strcpy(search, flags);
                            }
                            else if (target_directory == NULL)
                            {
                                target_directory = (char *)malloc(sizeof(char) * 256);
                                strcpy(target_directory, flags);
                            }
                            flags = strtok(NULL, " ");
                        }
                        if (executeFlag == 1 && (onlyDirs == 0 && onlyFiles == 0) || onlyDirs == 1 && onlyFiles == 1)
                        {
                            printf("INVALID flag\n");
                        }

                        else
                        {
                            if (search == NULL)
                            {
                                printf("No search term provided.\n");
                            }
                            else
                            {

                                if (target_directory == NULL)
                                {
                                    char current_directory[MAX_PATH_LENGTH];
                                    getcwd(current_directory, sizeof(current_directory));
                                    target_directory = current_directory;
                                }

                                search_directory_recursive(search, target_directory, onlyFiles, onlyDirs, executeFlag, "", 0);
                            }
                        }
                    }
                    else if (strcmp(command, "proclore") == 0)
                    {
                        proclore(command);
                    }
                    else if (strcmp(command, "iMan") == 0)
                    {
                        fetchManPage(token);
                    }
                    else if (strcmp(command, "activities") == 0)
                    {
                        listProcesses();
                    }
                    else if (strcmp(command, "ping") == 0)
                    {
                        char *pid_str = strtok(NULL, " ");
                        char *signal_str = strtok(NULL, " ");

                        if (pid_str == NULL || signal_str == NULL)
                        {
                            printf("Usage: ping <pid> <signal_number>\n");
                        }
                        else
                        {
                            int pid = atoi(pid_str);
                            int signal_number = atoi(signal_str);
                            ping(pid, signal_number);
                        }
                    }
                    else if (strcmp(command, "fg") == 0)
                    {
                        // Parse the pid from the command and call bringToForeground(pid)
                        char *pid_str = strtok(NULL, " ");
                        if (pid_str != NULL)
                        {
                            pid_t pid = atoi(pid_str);
                            bringToForeground(pid);
                        }
                        else
                        {
                            printf("Usage: fg <pid>\n");
                        }
                    }
                    else if (strcmp(command, "bg") == 0)
                    {
                        // Parse the pid from the command and call runInBackground(pid)
                        char *pid_str = strtok(NULL, " ");
                        if (pid_str != NULL)
                        {
                            pid_t pid = atoi(pid_str);
                            runInBackground(pid);
                        }
                        else
                        {
                            printf("Usage: bg <pid>\n");
                        }
                    }
                    else if (strcmp(command, "neonate") == 0)
                    {
                        char *arg = strtok(NULL, " ");
                        printneonate(arg);
                    }

                    else
                    {
                        if (counter == 1)
                        {
                            // printf("%s\n",string);
                            fgbg(string);
                            counter = 0;
                            // printf("%d\n", fgbgcheck);
                            fgbgcheck = 1;
                            // printf("%d\n", fgbgcheck);
                        }
                        else
                        {
                            function_execution(token, directory);
                        }
                    }
                }

                token = strtok_r(NULL, "&", &saveptr);
                saveHistory(&history);
                if (fgbgcheck == 1)
                {
                    fgbgcheck = 0;
                    break;
                }
            }
            token1 = strtok_r(NULL, ";", &saveptr1);
        }
    }
}
