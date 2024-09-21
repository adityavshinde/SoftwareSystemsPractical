/*
============================================================================
Name : 18.c
Author : Aditya Shinde
Description : Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
Date: 19th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipe1[2], pipe2[2];
    pipe(pipe1);  
    pipe(pipe2); 

    if (fork() == 0) { 
        dup2(pipe1[1], STDOUT_FILENO);
        close(pipe1[0]); close(pipe1[1]);
        execlp("ls", "ls", "-l", NULL);
        exit(1);
    }

    if (fork() == 0) { 
        dup2(pipe1[0], STDIN_FILENO);  
        dup2(pipe2[1], STDOUT_FILENO); 
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        execlp("grep", "grep", "^d", NULL);  
        exit(1);
    }

    close(pipe1[0]); close(pipe1[1]);

    if (fork() == 0) {  
        dup2(pipe2[0], STDIN_FILENO);  
        close(pipe2[0]); close(pipe2[1]);
        execlp("wc", "wc", "-l", NULL);  
        exit(1);
    }

    close(pipe2[0]); close(pipe2[1]);
    wait(NULL); wait(NULL); wait(NULL);  

    return 0;
}

/*
------------------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./18_out
0
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ls -l | grep ^d | wc -l
0
------------------------------------------------------------------------------------
*/
