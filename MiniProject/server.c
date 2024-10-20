#include <stdio.h> 
#include <errno.h> 

#include <fcntl.h>    
#include <unistd.h>    
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h>

#include <string.h> 
#include <stdbool.h>
#include <stdlib.h> 

#include "./functions/server-constants.h"
#include "./functions/admin.h"
#include "./functions/customer.h"
#include "./functions/employee.h"
#include "./functions/manager.h"

void connection_handler(int connFD); 

void main()
{
    int socketFD, socketBindStatus, socketListenStatus, connectionFD;
    struct sockaddr_in serverAddress, clientAddress;

    socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFD == -1)
    {
        perror("Error while creating server socket!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;            
    serverAddress.sin_port = htons(8081);            
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 

    socketBindStatus = bind(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (socketBindStatus == -1)
    {
        perror("Error while binding to server socket!");
        _exit(0);
    }

    socketListenStatus = listen(socketFD, 10);
    if (socketListenStatus == -1)
    {
        perror("Error while listening for connections on the server socket!");
        close(socketFD);
        _exit(0);
    }

    int clientSize;
    while (1)
    {
        clientSize = (int)sizeof(clientAddress);
        connectionFD = accept(socketFD, (struct sockaddr *)&clientAddress, &clientSize);
        if (connectionFD == -1)
        {
            perror("Error while connecting to client!");
            close(socketFD);
        }
        else
        {
            if (!fork())
            {
                connection_handler(connectionFD);
                close(connectionFD);
                _exit(0);
            }
        }
    }

    close(socketFD);
}

void connection_handler(int connectionFD)
{
    printf("Client has connected to the server!\n");

    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    int userChoice;

    writeBytes = write(connectionFD, INITIAL_PROMPT, strlen(INITIAL_PROMPT));
    if (writeBytes == -1)
        perror("Error while sending first prompt to the user!");
    else
    {
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
            perror("Error while reading from client");
        else if (readBytes == 0)
            printf("No data was sent by the client");
        else
        {
            userChoice = atoi(readBuffer);
            switch (userChoice)
            {
            case 1:
                admin_operation_handler(connectionFD);
                break;
            case 2:
                customer_operation_handler(connectionFD);
                break;
            case 3:
                employee_operation_handler(connectionFD);
                break;
            case 4:
                manager_operation_handler(connectionFD);
                break;
            default:
                break;
            }
        }
    }
    printf("Terminating connection to client!\n");
}
