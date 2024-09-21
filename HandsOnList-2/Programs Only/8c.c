/*
============================================================================
Name : 8c.c
Author : Aditya Shinde
Description : Write a separate program using signal system call to catch the following signals.
c. SIGFPE
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handle_sigfpe(int sig) {
    printf("Caught SIGFPE: Floating Point Exception\n");
    exit(1);
}

int main() {
    signal(SIGFPE, handle_sigfpe);

    int a = 1 / 0;  // Division by zero triggers SIGFPE

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8c_out
Caught SIGFPE: Floating Point Exception
---------------------------------------------------------------------
*/
