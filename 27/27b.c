/*
============================================================================
Name : 27b.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
b. execlp
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execlp:\n");

    // Execute ls -Rl
    execlp("ls", "ls", "-Rl", (char *)NULL);

    // If execlp returns, an error occurred
    perror("execlp");
    return 1;
}

