// ./server port_number

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define ARG_NUMBER 2
#define MAX_CONNECTIONS 5

int main(int argc, char *argv[])
{
    int serverfd, clientfd;
    struct sockaddr_in server, client;

    if (argc != ARG_NUMBER)
    {
        printf("Usage: %s port_number\n", argv[0]);
        exit(1);
    }

    // create the socket
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0)
    {
        fprintf(stderr, "Error: Failed to create the socket!\n");
        exit(1);
    }

    // bind the socket
    int port_number = atoi(argv[1]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port_number);

    if (bind(serverfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        fprintf(stderr, "Error: Failed to bind the server socket!\n");
        exit(1);
    }

    // listen for connections
    listen(serverfd, MAX_CONNECTIONS);

    // accept connections
    socklen_t size = sizeof(struct sockaddr_in);
    if ((clientfd = accept(serverfd, (struct sockaddr *)&client, &size)) < 0)
    {
        fprintf(stderr, "Error: Failed to accept connection!\n");
        exit(1);
    }

    while (1)
    {
        int n;
        char buffer[255];

        bzero(buffer, 255);
        if ((n = read(clientfd, buffer, 255) < 0))
        {
            fprintf(stderr, "Error: Read failed!\n");
            exit(1);
        }
        else
        {
            printf("%s", buffer);
        }
    }

    return 0;
}