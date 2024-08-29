/*
============================================================================
Name : 23.c
Author : Aditya Vijay Shinde
Description : Write a program to create a Zombie state of the running program.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid < 0) {
        // Fork failed
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child process PID: %d\n", getpid());
        sleep(30);  // Simulate a long-running child process
        printf("Child process terminating.\n");
        return 0;  // Exit normally
    } else {
        // Parent process
        printf("Parent process PID: %d\n", getpid());
        printf("Parent process will not call wait() immediately.\n");
        sleep(60);  // Sleep for 60 seconds to keep the child process in zombie state
        // Optionally, you can uncomment the following line to wait for the child process
        // wait(NULL);
        printf("Parent process exiting.\n");
        return 0;
    }
}

