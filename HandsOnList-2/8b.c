#include <stdio.h>
#include <signal.h>

void handle_sigint(int sig) {
    printf("Caught SIGINT (Ctrl + C)\n");
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);

    while (1) {
        // Waiting for Ctrl+C 
    }
    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8b_out
^CCaught SIGINT (Ctrl + C)
---------------------------------------------------------------------
*/

