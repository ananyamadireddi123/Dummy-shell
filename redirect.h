#ifndef REDIRECT
#define REDIRECT
#include "headers.h"
void out_direct(char* output_file,char* token,char* command,char* directory );

void in_direct1(char *input_file, char *token, char *command, char *directory , int pipe_fd[2]);
void in_direct(char *input_file, char *token, char *command, char *directory );




#endif