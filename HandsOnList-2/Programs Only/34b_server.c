/*
============================================================================
Name : 33b_server.c
Author : Aditya Shinde
Description : Write a program to create a concurrent server.
b. use pthread_create
Date: 21st Sep, 2024.
============================================================================
*/
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include <stdio.h>     
#include <unistd.h>     
#include <pthread.h>    


void communicate(int *fd)
{

    ssize_t bytesRead, bytesWrote;
    char msgFromClient[100];

    bytesWrote = write(*fd, "I'm the server!", 15);
    if (bytesWrote == -1)
        perror("Error while writing to network via socket!");
    else
        printf("Data sent to client!\n");

    bytesRead = read(*fd, msgFromClient, 100);
    if (bytesRead == -1)
        perror("Error while reading from network via socket!");
    else
        printf("Data from client: %s\n", msgFromClient);

    close(*fd);
}

void main()
{
    int sockFd, connectFd;
    int bindStat;
    int listenStat;
    int clientSize;

    struct sockaddr_in address, client;

    pthread_t threadID;

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd == -1)
    {
        perror("Error while creating socket!");
        _exit(0);
    }
    printf("Server side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    bindStat = bind(sockFd, (struct sockaddr *)&address, sizeof(address));
    if (bindStat == -1)
    {
        perror("Error while binding name to socket!");
        _exit(0);
    }
    printf("Binding to socket was successful!\n");

    listenStat = listen(sockFd, 2);
    if (listenStat == -1)
    {
        perror("Error while trying to listen for connections!");
        _exit(0);
    }
    printf("Now listening for connections on a socket!\n");

    while (1)
    {
        clientSize = (int)sizeof(client);
        connectFd = accept(sockFd, (struct sockaddr *)&client, &clientSize);
        if (connectFd == -1)
            perror("Error while accepting a connection!");
        else
        {   
            if (pthread_create(&threadID, NULL, (void *)communicate, &connectFd))
                perror("Error while creating thread");
        }
    }

    close(sockFd);
}

/*
-----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./34b_s_out
Server side socket successfully created!
Binding to socket was successful!
Now listening for connections on a socket!
Data sent to client!
Data from client: I'm the client!
-----------------------------------------------------------------------
*/
