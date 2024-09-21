/*
============================================================================
Name : 23.c
Author : Aditya Shinde
Description : Write a program to print the maximum number of files can be opened within a process and
size of a pipe (circular buffer).
Date: 20th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

int main() {
    long max_open_files = sysconf(_SC_OPEN_MAX);
    if (max_open_files == -1) {
        perror("sysconf funtion failed");
    } else {
        printf("Maximum number of open files: %ld\n", max_open_files);
    }

    long pipe_size = fpathconf(STDIN_FILENO, _PC_PIPE_BUF);
    if (pipe_size == -1) {
        perror("fpathconf function failed");
    } else {
        printf("Size of pipe : %ld bytes\n", pipe_size);
    }

    return 0;
}

/*
--------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./23_out
Maximum number of open files: 1024
Size of pipe : 4096 bytes
--------------------------------------------------------------
*/

