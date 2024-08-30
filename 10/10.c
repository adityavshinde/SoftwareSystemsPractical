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
    int fd = open("rwfile.txt", O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("Error in opening the file");
        exit(1);
    }

    char buffer1[10] = "HelloWorld";
    ssize_t bytes_written = write(fd, buffer1, 10);
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        exit(1);
    }

    off_t offset = lseek(fd, 10, SEEK_CUR);
    if (offset == -1) {
        perror("Error seeking in file");
        close(fd);
        exit(1);
    }
    printf("offset returned by lseek: %ld\n", offset);

    char buffer2[10] = "HelloWorld";
    bytes_written = write(fd, buffer2, 10);
    if (bytes_written < 0) {
        perror("Error writing to file");
        close(fd);
        exit(1);
    }

    close(fd);
    return 0;
}

/*
-----------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/10$ ./of10
offset returned by lseek: 20
-----------------------------------------------------------------

rwfile.txt - output:
HelloWorld\00\00\00\00\00\00\00\00\00\00HelloWorld

*/
