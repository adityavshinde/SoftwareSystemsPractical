/*
============================================================================
Name : 34a_client.c
Author : Aditya Shinde
Description : Write a program to create a concurrent server.
a. use fork
Date: 21st Sep, 2024.
============================================================================
*/
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h> 
#include <stdio.h>      
#include <unistd.h>     

void main()
{
    int sockFd; 
    int connectStat;       

    struct sockaddr_in address; 

    ssize_t bytesRead, bytesWrote; 
    char msgFromServer[100];

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd == -1)
    {
        perror("Error while creating socket!");
        _exit(0);
    }
    printf("Client side socket successfully created!\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    connectStat = connect(sockFd, (struct sockaddr *)&address, sizeof(address));
    if (connectStat == -1)
    {
        perror("Error while connecting to server!");
        _exit(0);
    }
    printf("Client to server connection successfully established!\n");


    bytesRead = read(sockFd, msgFromServer, 100);
    ;
    if (bytesRead == -1)
        perror("Error while reading from network via socket!");
    else
        printf("Data from server: %s\n", msgFromServer);

    bytesWrote = write(sockFd, "I'm the client!", 15);
    if (bytesWrote == -1)
        perror("Error while writing to network via socket!");
    else
        printf("Data sent to server!");
    
    close(sockFd);
}


/*
-----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./34a_c_out
Client side socket successfully created!
Client to server connection successfully established!
Data from server: I'm the server!
Data sent to server!
-----------------------------------------------------------------------
*/


