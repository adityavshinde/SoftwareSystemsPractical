/*
============================================================================
Name : 4.c
Author : Aditya Vijay Shinde
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also.
Date: 23rd Aug, 2024.
============================================================================
*/

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
    const char *filename = "file.txt";

    // Open file in read-write mode
    int fd = open(filename, O_RDWR);
    if (fd == -1) return perror("File open failed"), 1;
    close(fd);
    // Close after the first open

    // Try opening the file with O_EXCL
    fd = open(filename, O_RDWR | O_EXCL);
    if (fd == -1 && errno == EEXIST)
        return printf("O_EXCL failed: File exists.\n"), 1;

    printf("File has been opened successfully with O_EXCL.\n");
    close(fd);
    return 0;
}
/*
------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/4$ ls
file.txt  4.c  rw_of
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/4$ ./rw_of
File has been opened successfully with O_EXCL.
------------------------------------------------------
*/
