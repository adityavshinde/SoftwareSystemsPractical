/*
============================================================================
Name : 22.c
Author : Aditya Vijay Shinde
Description : Write a program, open a file, call fork, and then write to the 
file by both the child as well as the parent processes. Check output of the file.
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    // Open file for writing
    int fd = open("forkcall.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    // Fork a new process
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        perror("fork");
        close(fd);
        return 1;
    } else if (pid == 0) {
        // This is the child process
        const char *child_msg = "Message from child process.\n";
        write(fd, child_msg, strlen(child_msg));
        printf("Child Process: Written to file.\n");
    } else {
        // This is the parent process
        const char *parent_msg = "Message from parent process.\n";
        write(fd, parent_msg, strlen(parent_msg));
        printf("Parent Process: Written to file.\n");

        // Wait for the child process to complete
        wait(NULL);
    }

    // Close the file descriptor
    close(fd);
    return 0;
}

/*
------------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/22$ ./of22
Parent Process: Written to file.
Child Process: Written to file.
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/22$ ls
22.c  forkcall.txt  of22
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/22$ cat forkcall.txt
Message from parent process.
Message from child process.
------------------------------------------------------------------------
*/

