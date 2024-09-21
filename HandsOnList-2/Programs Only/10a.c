/*
============================================================================
Name : 10a.c
Author : Aditya Shinde
Description : Write a separate program using sigaction system call to catch the following signals.
a. SIGSEGV
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void handle_sigsegv(int sig) {
    printf("Caught SIGSEGV: Segmentation Fault\n");
    exit(1);  
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handle_sigsegv;  
    sigaction(SIGSEGV, &sa, NULL);   

    int *p = NULL;
    *p = 10;  

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./10a_out
Caught SIGSEGV: Segmentation Fault
--------------------------------------------------------------------
*/
