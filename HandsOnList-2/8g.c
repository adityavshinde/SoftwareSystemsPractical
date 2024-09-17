#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void handle_sigprof(int sig) {
    printf("Caught SIGPROF: Profiling timer triggered\n");
}

int main() {
    struct itimerval timer;
    signal(SIGPROF, handle_sigprof);

    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 0;

    setitimer(ITIMER_PROF, &timer, NULL);

    pause();

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT: g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8g_out
Caught SIGPROF: Profiling timer triggered
---------------------------------------------------------------------
*/
