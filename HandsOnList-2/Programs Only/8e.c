/*
============================================================================
Name : 8e.c
Author : Aditya Shinde
Description : Write a separate program using signal system call to catch the following signals.
e. SIGALRM (use setitimer system call)
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handle_sigalrm(int sig) {
    printf("Caught SIGALRM: Timer triggered using setitimer\n");
}

int main() {
    struct itimerval timer;
    signal(SIGALRM, handle_sigalrm);

    timer.it_value.tv_sec = 3;    
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;  
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timer, NULL);

    pause();

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8e_out
Caught SIGALRM: Timer triggered using setitimer
---------------------------------------------------------------------
*/
