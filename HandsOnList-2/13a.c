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

