/*
============================================================================
Name : 27d.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
d. execv
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execv:\n");

    // Arguments for ls -Rl
    char *args[] = {"ls", "-Rl", NULL};

    // Execute ls -Rl
    execv("/bin/ls", args);

    // If execv returns, an error occurred
    perror("execv");
    return 1;
}

