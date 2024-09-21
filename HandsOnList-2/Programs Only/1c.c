/*
============================================================================
Name : 1c.c
Author : Aditya Shinde
Description : Write a separate program (for each time domain) to set a 
interval timer in 10sec and 10micro second
c. ITIMER_PROF
Date: 16th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

void handle_sigprof(int signum) {
    printf("ITIMER_PROF timer expired!\n");
}

int main() {
    struct itimerval timer;

    signal(SIGPROF, handle_sigprof);

    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    setitimer(ITIMER_PROF, &timer, NULL);

    while(1) {
        pause();
    }

    return 0;
}

/*
--------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./1c_out
ITIMER_PROF timer expired!
^C
--------------------------------------------------------------------
