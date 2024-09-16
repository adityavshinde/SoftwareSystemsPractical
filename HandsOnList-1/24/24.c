/*
============================================================================
Name : 24.c
Author : Aditya Vijay Shinde
Description : Write a program to create an orphan process.
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();  // Create a new process

    if (pid < 0) {
        // Fork failed
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Child PID: %d\n", getpid());
        sleep(30);  // Keep the child process running for a while
        printf("Child process exiting.\n");
        return 0;
    } else {
        // Parent process
        printf("Parent PID: %d\n", getpid());
        sleep(5);  // Allow time for child to be running
        printf("Parent process exiting.\n");
        return 0;
    }
}

/*
-------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/24$ ./of24
Parent PID: 5406                                        //running for 5 seconds 
Child PID: 5407                                         //running for 30 seconds (total)
Parent process exiting.                                 //output after 5 seconds
Child process exiting.                                  //output after 25 seconds
-------------------------------------------------------------------
*/
