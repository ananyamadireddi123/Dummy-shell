#include "headers.h"

void setRawMode()
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

pid_t getMostRecentProcessID()
{
    DIR *dir;
    struct dirent *entry;
    pid_t most_recent_pid = 0;

    if ((dir = opendir("/proc")) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            const char *name = entry->d_name;
            int is_numeric = 1;
            for (int i = 0; name[i] != '\0'; i++)
            {
                if (!isdigit(name[i]))
                {
                    is_numeric = 0;
                    break;
                }
            }

            if (is_numeric)
            {
                pid_t pid = atoi(name);
                if (pid > 0 && (most_recent_pid == 0 || pid > most_recent_pid))
                {
                    most_recent_pid = pid;
                }
            }
        }
        closedir(dir);
    }
    else
    {
        perror("Unable to open /proc");
        exit(EXIT_FAILURE);
    }

    return most_recent_pid;
}

void printneonate(char *arg)
{
    if (arg != NULL && strcmp(arg, "-n") == 0)
    {
        arg = strtok(NULL, " ");
        if (arg != NULL)
        {
            int time_interval = atoi(arg);

            // Put the terminal in raw mode
            setRawMode();
            int x_pressed = 0;
            printf("# Press 'x' to exit\n");
            while (1)
            {

                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(STDIN_FILENO, &fds);

                struct timeval tv;
                tv.tv_sec = 0;       // 0 seconds
                tv.tv_usec = 100000; // 100 milliseconds

                int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

                if (ready == -1)
                {
                    perror("select");
                    break;
                }
                else if (ready > 0 && FD_ISSET(STDIN_FILENO, &fds))
                {
                    char c;
                    if (read(STDIN_FILENO, &c, 1) == 1)
                    {
                        if (c == 'x')
                        {
                            x_pressed = 1;
                            break;
                        }
                    }
                }

                pid_t recent_pid = getMostRecentProcessID();
                printf("%d\n", recent_pid);

                sleep(time_interval);
            }

            // Restore terminal settings
            struct termios term;
            tcgetattr(STDIN_FILENO, &term);
            term.c_lflag |= (ECHO | ICANON);
            tcsetattr(STDIN_FILENO, TCSANOW, &term);
        }
        else
        {
            printf("Usage: neonate -n [time_arg]\n");
        }
    }
    else
    {
        printf("Usage: neonate -n [time_arg]\n");
    }
}