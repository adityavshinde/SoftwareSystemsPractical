/*
============================================================================
Name : 26a.c
Author : Aditya Vijay Shinde
Description : Write a program to execute an executable program.
b. pass some input to an executable program. (for example execute an executable of $./a.out name)
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    printf("Parent Process PID: %d\n", getpid());

    // Execute ./a.out with an argument "Aditya"
    execl("./a.out", "a.out", "Aditya", (char *)NULL);    //execl( path, arg[0], arg[1], end )

    // If execl returns, an error occurred
    perror("execl");
    return 1;
}

/* 
------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/26$ ls
26a.c  26b.c  a.out  of26b
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/26$ ./of26b
Parent Process PID: 9815
Hello, Aditya!
------------------------------------------------------------------
*/
