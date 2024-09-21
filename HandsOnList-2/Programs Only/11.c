/*
============================================================================
Name : 11.c
Author : Aditya Shinde
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal -
use sigaction system call.
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    struct sigaction action;

    action.sa_handler = SIG_IGN;   
    sigaction(SIGINT, &action, NULL);

    printf("SIGINT is ignored for 5 seconds. Try pressing Ctrl+C.\n");
    sleep(5);

    action.sa_handler = SIG_DFL;  
    sigaction(SIGINT, &action, NULL);

    printf("SIGINT action is reset. Now Ctrl+C will terminate the program.\n");
    sleep(5);  

    return 0;
}

/*
-------------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./11_out
SIGINT is ignored for 5 seconds. Try pressing Ctrl+C.
^C^CSIGINT action is reset. Now Ctrl+C will terminate the program.
^C
-------------------------------------------------------------------------------
*/


