/*
============================================================================
Name : 33client.c
Author : Aditya Shinde
Description : Write a program to communicate between two machines using socket.
Date: 21st Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s_fd, new_sock;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buf[1024] = {0};

    s_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(s_fd, (struct sockaddr *)&address, sizeof(address));

    listen(s_fd, 3);

    new_sock = accept(s_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    read(new_sock, buf, sizeof(buf));

    printf("Message received: %s\n", buf);

    close(new_sock);
    close(s_fd);

    return 0;
}


/*
-----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./33cli_out
Hello Server
Message received: Hello from the client!
-----------------------------------------------------------------------
*/
