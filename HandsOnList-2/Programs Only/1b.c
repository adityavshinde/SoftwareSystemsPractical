/*
============================================================================
Name : 1b.c
Author : Aditya Shinde
Description : Write a separate program (for each time domain) to set a 
interval timer in 10sec and 10micro second
b. ITIMER_VIRTUAL
Date: 16th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void handle_sigvtalrm(int signum) {
    printf("ITIMER_VIRTUAL timer expired!\n");
}

int main() {
    struct itimerval timer;

    signal(SIGVTALRM, handle_sigvtalrm);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    while(1) {
        pause();
    }

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./1b_out
ITIMER_VIRTUAL timer expired!
^C
--------------------------------------------------------------------
*/

