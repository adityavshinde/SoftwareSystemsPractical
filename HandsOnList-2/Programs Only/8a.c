/*
============================================================================
Name : 8a.c
Author : Aditya Shinde
Description : Write a separate program using signal system call to catch the following signals.
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
    signal(SIGSEGV, handle_sigsegv);

    int *p = NULL;
    *p = 10;  //dereferencing null pointer

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8a_out
Caught SIGSEGV: Segmentation Fault
---------------------------------------------------------------------
*/

