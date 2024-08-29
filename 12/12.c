/*
============================================================================
Name : 12.c
Author : Aditya Vijay Shinde
Description : Write a program to find out the opening mode of a file. Use fcntl.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>   // For fcntl() and open()
#include <unistd.h>  // For close()

int main() {
    int fd = open("testfile.txt", O_RDWR); // Open file in read-write mode
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    int flags = fcntl(fd, F_GETFL);  // Get file status flags
    if (flags & O_RDWR) {
        printf("File is in read-write mode\n");
    } else if (flags & O_RDONLY) {
        printf("File is in read-only mode\n");
    } else if (flags & O_WRONLY) {
        printf("File is in write-only mode\n");
    }

    close(fd);  // Close the file
    return 0;
}

