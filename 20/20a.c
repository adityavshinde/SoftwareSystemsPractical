/*
============================================================================
Name : 20a.c
Author : Aditya Vijay Shinde
Description : Find out the priority of your running program.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();  // Get the PID of the current process

    // Get the priority of the current process
    int priority = getpriority(PRIO_PROCESS, pid);

    if (priority == -1) {
        perror("getpriority");
        return 1;
    }

    printf("Current priority of PID %d: %d\n", pid, priority);

    return 0;
}

