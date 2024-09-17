#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_sigalrm(int sig) {
    printf("Caught SIGALRM: Alarm triggered\n");
}

int main() {
    signal(SIGALRM, handle_sigalrm);

    alarm(5); //5 sec

    pause();  //wait

    return 0;
}

/*
---------------------------------------------------------------------
OUTPUT:g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./8d_out
Caught SIGALRM: Alarm triggered
---------------------------------------------------------------------
*/
