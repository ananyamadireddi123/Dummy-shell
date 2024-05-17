#include "headers.h"

int is_regular_file(const char *path)
{
    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
        return S_ISREG(path_stat.st_mode);
    }
    return 0;
}

int is_directory(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

void search_directory_recursive(const char *search, const char *directory, int onlyFiles, int onlyDirs, int executeFlag, const char *relative_path, int count)
{
    DIR *dir;
    if (directory != NULL)
    {
        dir = opendir(directory);
    }
    else
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    int avail = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue; // Skip "." and ".."
        }

        char entry_path[MAX_PATH_LENGTH];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", directory, entry->d_name);

        if (strstr(entry->d_name, search) != NULL)
        {
            avail = 1;

            if (!executeFlag & !onlyDirs)
            {
                char current_relative_path[MAX_PATH_LENGTH];
                snprintf(current_relative_path, sizeof(current_relative_path), "%s/%s", relative_path, entry->d_name);

                if (is_directory(entry_path))
                {
                    printf(ANSI_COLOR_BLUE); // Set color to blue for directories
                }
                else
                {
                    printf(ANSI_COLOR_GREEN); // Set color to green for files
                }

                // Print the entry's relative path
                printf("%s\n", current_relative_path);

                // Reset the color
                printf(ANSI_COLOR_RESET);
            }
        }

        if (is_directory(entry_path))
        {
            if (!executeFlag && (onlyFiles || onlyDirs))
            {
                // Perform actions based on the flags when executeFlag is false
                if (onlyFiles && is_regular_file(entry_path))
                {
                    printf("%s\n", entry_path);
                }
                else if (onlyDirs)
                {
                    printf("%s\n", entry_path);
                }
            }
            else if (executeFlag)
            {
                if (onlyFiles)
                {
                    // Extract the file name from the entry path
                    char *file_name = strrchr(entry_path, '/');
                    if (file_name != NULL)
                    {
                        file_name++; // Move past the '/'
                        printf("%s\n", file_name);
                    }
                    else
                    {
                        // If no '/' is found, the entry path is already the file name
                        printf("%s\n", entry_path);
                    }
                }
                else if (onlyDirs)
                {
                    if (chdir(entry_path) != 0)
                    {
                        perror("chdir");
                    }
                    else
                    {
                        char cwd[MAX_PATH_LENGTH];
                        if (getcwd(cwd, sizeof(cwd)) != NULL)
                        {
                            printf("%s\n", cwd);
                        }
                        else
                        {
                            perror("getcwd");
                        }
                    }
                }
                else if (onlyDirs && onlyFiles)
                {
                    printf("Invalid flags!!\n");
                }
            }
            else
            {
                char updated_relative_path[MAX_PATH_LENGTH];
                snprintf(updated_relative_path, sizeof(updated_relative_path), "%s/%s", relative_path, entry->d_name);

                search_directory_recursive(search, entry_path, onlyFiles, onlyDirs, executeFlag, updated_relative_path, count + 1);
            }
        }
    }

    if (avail == 0 && count == 0)
    {
        printf("NO matches found!\n");
    }

    closedir(dir);
}
