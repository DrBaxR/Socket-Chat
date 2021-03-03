// ./client server_address port_number username

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define ARG_NUMBER 4

int main(int argc, char *argv[])
{
    int serverfd;
    struct sockaddr_in server;

    if (argc != ARG_NUMBER)
    {
        printf("Usage: %s server_address port_number username\n", argv[0]);
        exit(1);
    }

    // create the socket
    if ((serverfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Error: Failed to create the socket!\n");
        exit(1);
    }

    // connect to the server
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));

    if (connect(serverfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        fprintf(stderr, "Error: Failed to connect to the remote server!\n");
        exit(1);
    }

    // send the client username
    write(serverfd, argv[3], strlen(argv[3]));

    // constantly send messages to the server
    while (1)
    {
        char message[255];

        // read MUST be done with fgets
        fgets(message, 255, stdin);

        // send the message
        if (write(serverfd, message, strlen(message)) < 0)
        {
            printf("Failed to send the message!\n");
        }
    }

    return 0;
}