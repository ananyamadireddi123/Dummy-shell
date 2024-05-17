#ifndef PAST
#define PAST

#define MAX_HISTORY_ENTRIES 15
#define HISTORY_FILE "history.txt"

typedef struct
{
    char commands[MAX_HISTORY_ENTRIES][4096];
    int count;
} History;


void loadHistory(History *history);
void saveHistory(History *history);
void addCommandToHistory(History *history, const char *command);
void executeCommand(const char *command);

#endif