#include "headers.h"

void fetchManPage(char *command_name)
{

    char *token = strtok(command_name, " ");

    // Check if we have at least two words
    if (token != NULL)
    {
        token = strtok(NULL, " ");
    }
    // Step 1: Perform DNS resolution manually

    struct hostent *host = gethostbyname("man.he.net");
    if (host == NULL)
    {
        perror("DNS resolution failed");
        return;
    }

    // Step 2: Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        return;
    }

    // Step 3: Set up the server address and connect to it
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port

    memcpy(&server_addr.sin_addr.s_addr, host->h_addr_list[0], host->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection to the server failed");
        close(sockfd);
        return;
    }

    // Step 4: Send an HTTP GET request
    char request[1024];
    snprintf(request, sizeof(request), "GET /?topic=%s&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n", token);
    // printf("%s\n",request);
    if (send(sockfd, request, strlen(request), 0) < 0)
    {
        perror("Sending request failed");
        close(sockfd);
        return;
    }

    // Step 5: Read the body of the website and remove HTML tags
    char buffer[1024];
    int bytes_received;
    int html_found = 0; // Flag to check if we are in HTML mode
    char plainTextBuffer[1024];
    char *output = plainTextBuffer;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';

        if (html_found == 0)
        {
            char *html_start = strstr(buffer, "<html>");
            if (html_start != NULL)
            {
                html_found = 1;
            }
        }

        if (html_found == 1)
        {
            int avail = 0;
            char *output = buffer; // Overwrite the input buffer while removing tags

            for (int i = 0; buffer[i] != '\0'; i++)
            {

                if (avail == 0)
                {
                    *output = buffer[i];
                    output++;
                }
                else if (buffer[i] == '<')
                {
                    avail = 1;
                }
                else if (buffer[i] == '>')
                {
                    avail = 0;
                }
            }

            *output = '\0'; // Null-terminate the string
            int len = strlen(buffer);

            int count = 0;
            int i;
            for (i = 0; count < 17; i++)
            {
                if (buffer[i] == '\n')
                {
                    count++;
                }
            }
            int j = 0;
            for (int k = i; k < len; k++)
            {
                buffer[j++] = buffer[k];
            }
            buffer[j] = '\0';
            if (strstr(buffer, "GNU coreutils") == NULL)
            {
                printf("%s", buffer);
            }
        }
        printf("\n");
    }

    // Close the socket

    close(sockfd);
}
