/*
============================================================================
Name : 14.c
Author : Aditya Shinde
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on
the monitor.
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    char write_msg[] = "Hello!";
    char read_msg[100];

    pipe(fd); 

    write(fd[1], write_msg, sizeof(write_msg));  
    read(fd[0], read_msg, sizeof(read_msg));     
    printf("Read from pipe: %s\n", read_msg);

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./14_out
Read from pipe: Hello!
--------------------------------------------------------------------
*/
