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
