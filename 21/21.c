/*
============================================================================
Name : 21.c
Author : Aditya Vijay Shinde
Description : Write a program, call fork and print the parent and child process id.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid < 0) {
        // Fork failed
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // This is the child process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
    } else {
        // This is the parent process
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);
    }

    return 0;
}

