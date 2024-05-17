#ifndef MAN
#define MAN
 

#define MAX_BUFFER_SIZE 1024

#define SERVER_HOST "man.he.net"
#define SERVER_PORT "80"  // HTTP default port

void fetchManPage(char *command_name);

#endif