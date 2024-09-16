/*
============================================================================
Name : 1c.c
Author : Aditya Vijay Shinde
Description : Create c.FIFO file using system call
Date: 21st Aug, 2024.
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main() {
    const char *fifofile = "/home/g3n1u5/SS/SoftwareSystemsPractical/FIFO/fifofile.txt";
    if (mkfifo(fifofile, 0666) == -1) {
        perror("mkfifo failed");
    } else {
        printf("FIFO has been created.\n");
    }
    return 0;
}

/*
---------------------------------------------------------------
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/FIFO$ ./outfile
FIFO has been created.
---------------------------------------------------------------
*/
