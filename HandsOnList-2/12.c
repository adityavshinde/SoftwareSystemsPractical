#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    if (fork() == 0) {
        sleep(1); 
        kill(getppid(), SIGKILL);  
        printf("Parent killed. Child (orphan) now adopted by systemd, new parent PID: %d\n", getppid());
    } else {
        sleep(10); 
    }
    return 0;
}

/*
-------------------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./12_out
Parent killed. Child (orphan) now adopted by systemd, new parent PID: 92755
Killed
-------------------------------------------------------------------------------------
*/
