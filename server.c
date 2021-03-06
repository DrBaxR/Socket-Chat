// ./server port_number

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define ARG_NUMBER 2
#define MAX_CONNECTIONS 5

int client_fds[1000];
pthread_t thread_ids[1000];
int client_number = 0;

void log_done(char* string) {
    printf("\033[1;32mDONE:\033[0m %s\n", string);
}

void log_error(char* string) {
    printf("\033[1;31mERROR:\033[0m %s\n", string);
}

void log_name(char* string) {
    printf("\033[1;33m%s:\033[0m ", string);
}

void *client_handler(void *args)
{
    int client_index = *(int *)args;

    // read username
    int n;
    char username[256];

    bzero(username, 256);

    if ((n = read(client_fds[client_index], username, 255)) < 0)
    {
        printf("Failed to read the username!\n");
    }
    else
    {
        printf("\033[1;34m%s\033[0m\033[0;34m connected to the room!\033[0m\n", username);
    }

    // read user messages
    char buffer[256];

    while ((n = read(client_fds[client_index], buffer, 255)))
    {
        if (n < 0)
            printf("Failed to receive message!\n");
        else {
            log_name(username);
            printf("%s", buffer);
        }

        bzero(buffer, 255);
    }

    // print message when user disconnects
    printf("\033[1;35m%s\033[0m\033[0;35m disconnected from the room!\033[0m\n", username);

    return NULL;
}

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
        log_error("Failed to create the socket!");
        exit(1);
    }

    log_done("Created socket.");

    // bind the socket
    int port_number = atoi(argv[1]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port_number);

    if (bind(serverfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        log_error("Failed to bind the server socket!");
        exit(1);
    }

    log_done("Bound server socket.");

    // listen for connections
    listen(serverfd, MAX_CONNECTIONS);

    log_done("Server listening for connections...");

    // accept connections
    socklen_t size = sizeof(struct sockaddr_in);

    while ((clientfd = accept(serverfd, (struct sockaddr *)&client, &size)))
    {
        // TODO: cand primeste un mesaj, arata mesajul in server si trimite mesajul primit la toti ceilalti clienti (modificari si la server si la client)
        // TODO: dat join la thread-uri cand se termina

        if (clientfd < 0)
        {
            log_error("Failed to accept connection!");
            exit(1);
        }

        // save the client file descriptor in the array
        client_fds[client_number] = clientfd;

        int *index_pointer = (int *)malloc(sizeof(int));
        *index_pointer = client_number;

        pthread_create(&thread_ids[client_number], NULL, client_handler, index_pointer);
        client_number++;
    }

    return 0;
}