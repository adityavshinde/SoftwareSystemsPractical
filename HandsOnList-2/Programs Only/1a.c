/*
============================================================================
Name : 1a.c
Author : Aditya Shinde
Description : Write a separate program (for each time domain) to set a 
interval timer in 10sec and 10micro second
a. ITIMER_REAL
Date: 16th Sep, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void handle_sigalrm(int signum) {
    printf("ITIMER_REAL timer expired!\n");
}

int main() {
    struct itimerval timer;

    signal(SIGALRM, handle_sigalrm);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    setitimer(ITIMER_REAL, &timer, NULL);

    while(1) {
        pause();
    }

    return 0;
}

/*
------------------------------------------------------------------------
OUTPUT:

g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./1a_out
ITIMER_REAL timer expired!
ITIMER_REAL timer expired!
^C

*/
