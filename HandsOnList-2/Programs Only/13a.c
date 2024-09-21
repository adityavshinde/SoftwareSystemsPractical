/*
============================================================================
Name : 13a.c
Author : Aditya Shinde
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program
will send the signal (using kill system call). Find out whether the first program is able to catch
the signal or not.
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigcont(int sig) {
    printf("Caught SIGCONT: Process resumed after SIGSTOP\n");
}

int main() {
    signal(SIGCONT, handle_sigcont);

    printf("Process PID: %d\n", getpid());
    printf("Waiting for SIGSTOP (cannot catch SIGSTOP)\n");

    while (1) {
        pause();  
    }

    return 0;
}

/*
-----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./13a_out
Process PID: 94262
Waiting for SIGSTOP (cannot catch SIGSTOP)

[1]+  Stopped                 ./13a_out
aught SIGCONT: Process resumed after SIGSTOP
-----------------------------------------------------------------------
*/

