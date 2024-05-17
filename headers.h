#ifndef HEADERS_H_
#define HEADERS_H_

#define MAX_COMMANDS 10
#define MAX_COMMAND_LENGTH 100
#define MAX_ARGS 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <dirent.h>

#include <signal.h>


#include <sys/time.h>
#include <sys/resource.h>
#include <fcntl.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
 

//extern char* lastbutone;

#include <termios.h>
#include <fcntl.h>


#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "prompt.h"
#include "function.h"
#include "warp.h"
#include "proc.h"
#include "past.h"
#include "peek.h"
#include "seek.h"
#include "man.h"
#include "act.h"
#include "ping.h"
#include "redirect.h"
#include "neonate.h"
#include "fg.h"

extern pid_t foreground_pid;
extern pid_t background_pid;

#endif