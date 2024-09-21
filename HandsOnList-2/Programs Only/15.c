/*
============================================================================
Name : 15.c
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
    pipe(fd);  

    if (fork() == 0) { 
        char buf[100];
        close(fd[1]);  
        read(fd[0], buf, sizeof(buf));  
        printf("Child received: %s\n", buf);
        close(fd[0]); 
    } 
    else {  
        char wr[] = "Hello parent here!\n";
        close(fd[0]);  
        write(fd[1], wr, sizeof(wr));  
        close(fd[1]); 
    }

    return 0;
}

/*
-------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./15_out
Child received: Hello parent here!

--------------------------------------------------------------------
*/
