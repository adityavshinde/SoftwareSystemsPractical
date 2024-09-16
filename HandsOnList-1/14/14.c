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

    struct stat s;

    // Get file information using stat()
    if (stat(argv[1], &s) < 0) {
        perror("Error getting file information");
        exit(1);
    }

    // Identify the file type
    if (S_ISREG(s.st_mode)) {
        printf("The file '%s' is a regular file.\n", argv[1]);
    } 
    else if (S_ISDIR(s.st_mode)) {
        printf("The file '%s' is a directory.\n", argv[1]);
    } 
    else if (S_ISLNK(s.st_mode)) {
        printf("The file '%s' is a symbolic link.\n", argv[1]);
    } 
    else if (S_ISCHR(s.st_mode)) {
        printf("The file '%s' is a character device.\n", argv[1]);
    } 
    else if (S_ISBLK(s.st_mode)) {
        printf("The file '%s' is a block device.\n", argv[1]);
    } 
    else if (S_ISFIFO(s.st_mode)) {
        printf("The file '%s' is a FIFO (named pipe).\n", argv[1]);
    } 
    else if (S_ISSOCK(s.st_mode)) {
        printf("The file '%s' is a socket.\n", argv[1]);
    } 
    else {
        printf("The file '%s' type is unknown.\n", argv[1]);
    }

    return 0;
}

/*
-----------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ ls
14.c  of14
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ mkdir directory
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ mkfifo fifofile
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ ls
14.c  directory  fifofile  of14
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ ./of14 directory
The file 'directory' is a directory.
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ ./of14 fifofile
The file 'fifofile' is a FIFO (named pipe).
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/14$ ./of14
Usage: ./of14 <filename>
----------------------------------------------------------------------
*/
