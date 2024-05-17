#include "headers.h"

void printColored(const char *text, const char *color)
{
    printf("%s%s\033[0m", color, text);
}


int compareEntries(const struct dirent **a, const struct dirent **b)
{
    return strcmp((*a)->d_name, (*b)->d_name);
}

void listFilesAndDirectories(const char *path, int showHidden, int showDetails)
{
    struct dirent **entries;
    int entryCount = scandir(path, &entries, NULL, compareEntries);

    if (entryCount < 0)
    {
        perror("Error scanning directory");
        return;
    }

    for (int i = 0; i < entryCount; i++)
    {
        if (!showHidden && entries[i]->d_name[0] == '.')
        {
            continue; // Skip hidden files
        }

        struct stat entryStat;
        char fullPath[4096];
        getcwd(fullPath,4096);
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entries[i]->d_name);
        stat(fullPath, &entryStat);

        if (showDetails)
        {
            char permissions[11];
            permissions[0] = (S_ISDIR(entryStat.st_mode)) ? 'd' : '-';
            permissions[1] = (entryStat.st_mode & S_IRUSR) ? 'r' : '-';
            permissions[2] = (entryStat.st_mode & S_IWUSR) ? 'w' : '-';
            permissions[3] = (entryStat.st_mode & S_IXUSR) ? 'x' : '-';
            permissions[4] = (entryStat.st_mode & S_IRGRP) ? 'r' : '-';
            permissions[5] = (entryStat.st_mode & S_IWGRP) ? 'w' : '-';
            permissions[6] = (entryStat.st_mode & S_IXGRP) ? 'x' : '-';
            permissions[7] = (entryStat.st_mode & S_IROTH) ? 'r' : '-';
            permissions[8] = (entryStat.st_mode & S_IWOTH) ? 'w' : '-';
            permissions[9] = (entryStat.st_mode & S_IXOTH) ? 'x' : '-';
            permissions[10] = '\0';

            struct passwd *user = getpwuid(entryStat.st_uid);
            struct group *group = getgrgid(entryStat.st_gid);

            char timeBuffer[80];
            strftime(timeBuffer, sizeof(timeBuffer), "%b %d %H:%M", localtime(&entryStat.st_mtime));

            printf("%s %2lu %-8s %-8s %8ld %s %s\n",
                   permissions, entryStat.st_nlink,
                   user ? user->pw_name : "unknown",
                   group ? group->gr_name : "unknown",
                   entryStat.st_size, timeBuffer,
                   entries[i]->d_name);
        }
        else
        {
            if (S_ISDIR(entryStat.st_mode))
            {
                printColored(entries[i]->d_name, "\033[34m"); // Blue for directories
            }
            else if (entryStat.st_mode & S_IXUSR)
            {
                printColored(entries[i]->d_name, "\033[32m"); // Green for executables
            }
            else
            {
                printf("%s", entries[i]->d_name); // Default white color
            }
            printf("\n");
        }
    }

    for (int i = 0; i < entryCount; i++)
    {
        free(entries[i]);
    }
    free(entries);
}















// #include "headers.h"

// int compareEntries(const void *a, const void *b)
// {
//     return strcmp(((struct dirent *)a)->d_name, ((struct dirent *)b)->d_name);
// }

// void printColored(const char *text, const char *color)
// {
//     printf("%s%s\033[0m", color, text);
// }

// void listFilesAndDirectories(const char *path, int showHidden, int showDetails)
// {
//     DIR *dir;
//     struct dirent *entry;
//     struct dirent **entries;

//     dir = opendir(path);

//     if (dir == NULL)
//     {
//         perror("Error opening directory");
//         return;
//     }

//     int entryCount = 0;
//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (!showHidden && entry->d_name[0] == '.')
//         {
//             continue; // Skip hidden files
//         }
//         entryCount++;
//     }

//     rewinddir(dir);
//     entries = (struct dirent **)malloc(entryCount * sizeof(struct dirent *));
//     entryCount = 0;
//     while ((entry = readdir(dir)) != NULL)
//     {
//         if (!showHidden && entry->d_name[0] == '.')
//         {
//             continue; // Skip hidden files
//         }
//         entries[entryCount++] = entry;
//     }
//     closedir(dir);

//     // Sort the entries lexicographically
//     // for (int i = 0; i < entryCount - 1; i++)
//     // {
//     //     for (int j = i + 1; j < entryCount; j++)
//     //     {
//     //         if (strcmp(entries[i]->d_name, entries[j]->d_name) > 0)
//     //         {
//     //             struct dirent *temp = entries[i];
//     //             entries[i] = entries[j];
//     //             entries[j] = temp;
//     //         }
//     //     }
//     // }

//     qsort(entries, entryCount, sizeof(struct dirent *), compareEntries);

//     for (int i = 0; i < entryCount; i++)
//     {
//         struct stat entryStat;
//         char fullPath[4096];
//         snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entries[i]->d_name);
//         stat(fullPath, &entryStat);

//         if (showDetails)
//         {

//             // ...
//             // Display detailed information similar to 'ls -l'
//             char permissions[11];
//             permissions[0] = (S_ISDIR(entryStat.st_mode)) ? 'd' : '-';
//             permissions[1] = (entryStat.st_mode & S_IRUSR) ? 'r' : '-';
//             permissions[2] = (entryStat.st_mode & S_IWUSR) ? 'w' : '-';
//             permissions[3] = (entryStat.st_mode & S_IXUSR) ? 'x' : '-';
//             permissions[4] = (entryStat.st_mode & S_IRGRP) ? 'r' : '-';
//             permissions[5] = (entryStat.st_mode & S_IWGRP) ? 'w' : '-';
//             permissions[6] = (entryStat.st_mode & S_IXGRP) ? 'x' : '-';
//             permissions[7] = (entryStat.st_mode & S_IROTH) ? 'r' : '-';
//             permissions[8] = (entryStat.st_mode & S_IWOTH) ? 'w' : '-';
//             permissions[9] = (entryStat.st_mode & S_IXOTH) ? 'x' : '-';
//             permissions[10] = '\0';

//             struct passwd *user = getpwuid(entryStat.st_uid);
//             struct group *group = getgrgid(entryStat.st_gid);

//             char timeBuffer[80];
//             strftime(timeBuffer, sizeof(timeBuffer), "%b %d %H:%M", localtime(&entryStat.st_mtime));

//             printf("%s %2lu %-8s %-8s %8ld %s %s\n",
//                    permissions, entryStat.st_nlink,
//                    user ? user->pw_name : "unknown",
//                    group ? group->gr_name : "unknown",
//                    entryStat.st_size, timeBuffer,
//                    entry->d_name);
//         }
//         else
//         {
//             // Display only file/directory names with color
//             // ...
//             if (S_ISDIR(entryStat.st_mode))
//             {
//                 printColored(entry->d_name, "\033[34m"); // Blue for directories
//             }
//             else if (entryStat.st_mode & S_IXUSR)
//             {
//                 printColored(entry->d_name, "\033[32m"); // Green for executables
//             }
//             else
//             {
//                 printf("%s", entry->d_name); // Default white color
//             }
//             printf("\n");
//         }
//     }

//     free(entries);
// }
