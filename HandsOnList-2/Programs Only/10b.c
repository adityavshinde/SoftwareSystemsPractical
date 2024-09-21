/*
============================================================================
Name : 10b.c
Author : Aditya Shinde
Description : Write a separate program using sigaction system call to catch the following signals.
b. SIGINT
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("Caught SIGINT: Interrupt from keyboard (Ctrl + C)\n");
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;   
    sigaction(SIGINT, &sa, NULL);   

    while (1);

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./10b_out
^CCaught SIGINT: Interrupt from keyboard (Ctrl + C)
--------------------------------------------------------------------
*/
