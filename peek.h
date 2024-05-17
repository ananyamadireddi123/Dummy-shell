#ifndef PEEK
#define PEEK
int compareEntries(const struct dirent **a, const struct dirent **b);
void printColored(const char *text, const char *color);
void listFilesAndDirectories(const char *path, int showHidden, int showDetails);

#endif