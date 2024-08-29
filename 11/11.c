/*
============================================================================
Name : 11.c
Author : Aditya Vijay Shinde
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
a. use dup
b. use dup2
c. use fcntl
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>  // For open(), fcntl(), O_WRONLY
#include <unistd.h> // For dup(), dup2(), write(), close()

int main() {
    // Open the file in append mode (write only, create if it doesn't exist)
    int fd = open("testfile.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    // 1. Use dup to duplicate the file descriptor
    int fd_dup = dup(fd);
    if (fd_dup < 0) {
        perror("Error duplicating file descriptor with dup()");
        close(fd);
        return 1;
    }

    // Write using the original descriptor
    write(fd, "Original descriptor\n", 20);

    // Write using the duplicated descriptor
    write(fd_dup, "Dup descriptor\n", 15);

    // 2. Use dup2 to duplicate the file descriptor to a specific descriptor number (let's say 10)
    int fd_dup2 = dup2(fd, 10);
    if (fd_dup2 < 0) {
        perror("Error duplicating file descriptor with dup2()");
        close(fd);
        close(fd_dup);
        return 1;
    }

    // Write using the fd duplicated by dup2
    write(fd_dup2, "Dup2 descriptor\n", 16);

    // 3. Use fcntl to duplicate the file descriptor
    int fd_fcntl = fcntl(fd, F_DUPFD, 0);
    if (fd_fcntl < 0) {
        perror("Error duplicating file descriptor with fcntl()");
        close(fd);
        close(fd_dup);
        close(fd_dup2);
        return 1;
    }

    // Write using the fd duplicated by fcntl
    write(fd_fcntl, "Fcntl descriptor\n", 17);

    // Close all file descriptors
    close(fd);
    close(fd_dup);
    close(fd_dup2);
    close(fd_fcntl);

    return 0;
}

