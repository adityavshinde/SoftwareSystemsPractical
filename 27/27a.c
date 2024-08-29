/*
============================================================================
Name : 27a.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
a. execl
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

#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execl:\n");

    // Execute ls -Rl
    execl("/bin/ls", "ls", "-Rl", (char *)NULL);

    // If execl returns, an error occurred
    perror("execl");
    return 1;
}

