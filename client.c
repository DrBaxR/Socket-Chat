// ./client server_address port_number username

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

#define ARG_NUMBER 4

pthread_t thread_id;
int serverfd;

void log_done(char *string)
{
    printf("\033[1;32mDONE:\033[0m %s\n", string);
}

void log_error(char *string)
{
    printf("\033[1;31mERROR:\033[0m %s\n", string);
}

void log_name(char *string)
{
    printf("\033[1;33m%s:\033[0m ", string);
}

void *server_read_handler(void *args)
{
    // constantly read data from the server (username first, messsage second <- for each message)
    int n1, n2;
    char username[256];
    char message[256];

    bzero(username, 256);
    while ((n1 = read(serverfd, username, 256)) > 0)
    {
        printf("%s", username);
        // if ((n2 = read(serverfd, message, 256)) < 0)
        // {
        //     log_error("Failed to read message from server!");
        // }
        // else
        // {
        //     printf("Message: %s", message);
            
        //     username[strlen(username) - 1] = '\0';
        //     message[strlen(message) - 1] = '\0';

        //     log_name(username);
        //     printf("%s\n", message);

        //     bzero(username, 256);
        //     bzero(message, 256);
        // }

        bzero(username, 256);
        bzero(message, 256);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
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

    pthread_create(&thread_id, NULL, server_read_handler, NULL);

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