/*
============================================================================
Name : 25.c
Author : Aditya Vijay Shinde
Description : Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid1, pid2, pid3;

    // Create the first child process
    if ((pid1 = fork()) == 0) {
        // First child
        printf("First Child PID: %d\n", getpid());
        exit(0);  // Exit child process
    }

    // Create the second child process
    if ((pid2 = fork()) == 0) {
        // Second child
        printf("Second Child PID: %d\n", getpid());
        exit(0);  // Exit child process
    }

    // Create the third child process
    if ((pid3 = fork()) == 0) {
        // Third child
        printf("Third Child PID: %d\n", getpid());
        exit(0);  // Exit child process
    }

    // Parent process
    printf("Parent PID: %d\n", getpid());
    printf("Waiting for second child with PID: %d\n", pid2);

    // Wait for the second child process
    int status;
    waitpid(pid2, &status, 0);

    // Check if the child exited normally
    if (WIFEXITED(status)) {
        printf("Second child exited with status %d\n", WEXITSTATUS(status));
    }

    // Wait for any remaining child processes
    while (wait(NULL) > 0);

    printf("All child processes have terminated.\n");

    return 0;
}

