/*
============================================================================
Name : 27e.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
e. execvp
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execvp:\n");

    // Arguments for ls -Rl
    char *args[] = {"ls", "-Rl", NULL};

    // Execute ls -Rl
    execvp("ls", args);

    // If execvp returns, an error occurred
    perror("execvp");
    return 1;
}

