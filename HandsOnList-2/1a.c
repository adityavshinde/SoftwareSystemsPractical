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

    // Register signal handler for SIGALRM
    signal(SIGALRM, handle_sigalrm);

    // Set the interval timer to 10 seconds and 10 microseconds
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 10;
    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 10;

    // Start the ITIMER_REAL timer
    setitimer(ITIMER_REAL, &timer, NULL);

    // Infinite loop to keep the program running
    while(1) {
        pause(); // Wait for the signal
    }

    return 0;
}
