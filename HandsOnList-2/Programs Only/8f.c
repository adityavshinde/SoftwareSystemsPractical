/*
============================================================================
Name : 8f.c
Author : Aditya Shinde
Description : Write a separate program using signal system call to catch the following signals.
f. SIGVTALRM (use setitimer system call)
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handle_sigvtalrm(int sig) {
    printf("Caught SIGVTALRM: Virtual timer triggered\n");
}

int main() {
    struct itimerval timer;
    signal(SIGVTALRM, handle_sigvtalrm);

    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    pause();
    
    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8f_out
Caught SIGVTALRM: Virtual timer triggered
---------------------------------------------------------------------
*/
