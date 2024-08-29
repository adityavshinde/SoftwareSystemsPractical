/*
============================================================================
Name : 14.c
Author : Aditya Vijay Shinde
Description : Write a program to find the type of a file.
a. Input should be taken from command line.
b. program should be able to identify any type of a file.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <sys/stat.h>  // For stat() and struct stat
#include <stdlib.h>    // For exit()

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    struct stat fileStat;

    // Get file information using stat()
    if (stat(argv[1], &fileStat) < 0) {
        perror("Error getting file information");
        exit(1);
    }

    // Identify the file type
    if (S_ISREG(fileStat.st_mode)) {
        printf("The file '%s' is a regular file.\n", argv[1]);
    } else if (S_ISDIR(fileStat.st_mode)) {
        printf("The file '%s' is a directory.\n", argv[1]);
    } else if (S_ISLNK(fileStat.st_mode)) {
        printf("The file '%s' is a symbolic link.\n", argv[1]);
    } else if (S_ISCHR(fileStat.st_mode)) {
        printf("The file '%s' is a character device.\n", argv[1]);
    } else if (S_ISBLK(fileStat.st_mode)) {
        printf("The file '%s' is a block device.\n", argv[1]);
    } else if (S_ISFIFO(fileStat.st_mode)) {
        printf("The file '%s' is a FIFO (named pipe).\n", argv[1]);
    } else if (S_ISSOCK(fileStat.st_mode)) {
        printf("The file '%s' is a socket.\n", argv[1]);
    } else {
        printf("The file '%s' type is unknown.\n", argv[1]);
    }

    return 0;
}

