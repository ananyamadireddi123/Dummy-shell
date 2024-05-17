#include "headers.h"

void loadHistory(History *history)
{
    FILE *file = fopen(HISTORY_FILE, "r");
    if (file)
    {
        while (fgets(history->commands[history->count], sizeof(history->commands[0]), file))
        {
            history->commands[history->count][strcspn(history->commands[history->count], "\n")] = '\0';
            history->count++;
        }
        fclose(file);
    }
}

void saveHistory(History *history)
{
    FILE *file = fopen(HISTORY_FILE, "w");
    if (file)
    {
        for (int i = 0; i < history->count; i++)
        {
            fprintf(file, "%s\n", history->commands[i]);
        }
        fclose(file);
    }
}

void addCommandToHistory(History *history, const char *command)
{
    if (history->count == MAX_HISTORY_ENTRIES)
    {
        for (int i = 1; i < MAX_HISTORY_ENTRIES; i++)
        {
            strcpy(history->commands[i - 1], history->commands[i]);
        }
        history->count--;
    }
    strcpy(history->commands[history->count], command);
    history->count++;
}

void executeCommand(const char *command)
{
    // Implement command execution here
    system(command);
}