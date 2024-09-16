/*
============================================================================
Name : 23.c
Author : Aditya Vijay Shinde
Description : Write a program to create a Zombie state of the running program.
Date: 30th Aug, 2024.
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
        sleep(60);  // Sleep for 60 seconds to keep the child process for 30 s ( 31 to 60) in zombie state
        printf("Parent process exiting.\n");
        return 0;
    }
}
/*
----------------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/23$ ./of23
Parent process PID: 5216
Parent process will not call wait() immediately.
Child process PID: 5217
Child process terminating.               //after 30 seconds
Parent process exiting.                  //after 60 seconds
----------------------------------------------------------------------------
*/
