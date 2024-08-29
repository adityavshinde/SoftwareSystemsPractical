/*
============================================================================
Name : 27c.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
c. execle
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execle:\n");

    // Execute ls -Rl with environment variables
    execle("/bin/ls", "ls", "-Rl", (char *)NULL, NULL);

    // If execle returns, an error occurred
    perror("execle");
    return 1;
}

