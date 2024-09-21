/*
============================================================================
Name : 12.c
Author : Aditya Shinde
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to
the parent process from the child process.
Date: 18th Sep, 2024.
============================================================================
*/
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
