/*
============================================================================
Name : 9.c
Author : Aditya Shinde
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT
signal - Use signal system call.
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void ignore_sigint(int sig) {
    printf("SIGINT is currently ignored\n");
}

int main() {
    // Ignore SIGINT
    signal(SIGINT, SIG_IGN);
    printf("SIGINT is being ignored now. Try pressing Ctrl+C within 5 seconds\n");

    sleep(5);

    signal(SIGINT, SIG_DFL);
    printf("SIGINT action reset to default. Try pressing Ctrl+C again within 5 seconds\n");

    sleep(5);

    return 0;
}

/*
-----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./9_out
SIGINT is now ignored. Try pressing Ctrl+C.
^C^C^C^CSIGINT action reset to default. Try pressing Ctrl+C again.
^C
-----------------------------------------------------------------------
*/
