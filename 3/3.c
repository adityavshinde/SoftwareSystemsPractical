/*
===========================================================================================================
Name : 3.c
Author : Aditya Vijay Shinde
Description : Write a program to create a file and print the file descriptor value. Use creat() system call
Date: 23rd Aug, 2024.
===========================================================================================================
*/

#include <fcntl.h>   // For creat()
#include <stdio.h>

int main() {
    const char *filename = "example.txt";

    // Create the file with read and write permissions for the owner
    int fd = creat(filename, 0644);

    if (fd == -1) {
        perror("File creation failed");
    } else {
        printf("File descriptor value: %d\n", fd);
    }

    // Optionally close the file after use
    pclose(fd);

    return 0;
}

//output:
//File descriptor value: 3
