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

    // Execute ./a.out with an argument "Alice"
    execl("./a.out", "a.out", "Alice", (char *)NULL);

    // If execl returns, an error occurred
    perror("execl");
    return 1;
}

