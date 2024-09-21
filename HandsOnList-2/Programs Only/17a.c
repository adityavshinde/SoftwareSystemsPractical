/*
============================================================================
Name : 17a.c
Author : Aditya Shinde
Description : Write a program to execute ls -l | wc.
a. use dup
Date: 19th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2];
    pipe(fd);  

    if (fork() == 0) {  
        close(fd[0]);  
        close(STDOUT_FILENO);
        dup(fd[1]);  
        close(fd[1]);
        execlp("ls", "ls", "-l", NULL); 
    } else {  
        close(fd[1]);  
        close(STDIN_FILENO);
        dup(fd[0]);  
        close(fd[0]);
        execlp("wc", "wc", NULL);  
    }

    return 0;
}


/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./17c_out
     72     641    3698
---------------------------------------------------------------------
*/
