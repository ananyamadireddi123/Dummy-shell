#ifndef SEEK
#define SEEK

#define MAX_PATH_LENGTH 257

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

int is_directory(const char *path);
void search_directory_recursive(const char *search,const char *directory, int onlyFiles, int onlyDirs, int executeFlag,const char* relative_path,int count);
 



#endif