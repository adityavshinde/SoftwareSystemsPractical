/*
============================================================================
Name : 10.c
Author : Aditya Vijay Shinde
Description : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
a. check the return value of lseek
b. open the file with od and check the empty spaces in between the data.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <fcntl.h>   // For open()
#include <unistd.h>  // For lseek(), write(), close()
#include <stdlib.h>  // For exit()

int main() {
    // Open the file in read-write mode, create if it doesn't exist
    int fd = open("testfile.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }

    // Write 10 bytes
    char buffer1[10] = "ABCDEFGHIJ";
    ssize_t bytes_written = write(fd, buffer1, 10);
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        exit(1);
    }

    // Move the file pointer forward by 10 bytes using lseek
    off_t offset = lseek(fd, 10, SEEK_CUR);
    if (offset == -1) {
        perror("Error seeking in file");
        close(fd);
        exit(1);
    }
    printf("lseek returned offset: %ld\n", offset);

    // Write another 10 bytes
    char buffer2[10] = "1234567890";
    bytes_written = write(fd, buffer2, 10);
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        exit(1);
    }

    // Close the file
    close(fd);
    return 0;
}
