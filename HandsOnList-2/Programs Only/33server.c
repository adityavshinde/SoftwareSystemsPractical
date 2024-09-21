/*
============================================================================
Name : 33server.c
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
    int sock = 0;
    struct sockaddr_in s_addr;
    char *message = "Hello from the client!";

    sock = socket(AF_INET, SOCK_STREAM, 0);

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &s_addr.sin_addr);

    connect(sock, (struct sockaddr *)&s_addr, sizeof(s_addr));

    send(sock, message, strlen(message), 0);

    close(sock);

    return 0;
}



