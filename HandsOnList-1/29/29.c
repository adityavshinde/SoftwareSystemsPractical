/*
============================================================================
Name : 29.c
Author : Aditya Vijay Shinde
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,SCHED_RR).
Date: 31st Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <sched.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();  // Get process ID

    // Get and print current scheduling policy
    int policy = sched_getscheduler(pid);
    printf("Current policy: %d\n", policy);
    switch (policy) {
        case SCHED_FIFO:
            printf("Current policy: SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("Current policy: SCHED_RR\n");
            break;
        case SCHED_OTHER:
            printf("Current policy: SCHED_OTHER\n");
            break;
        default:
            printf("Current policy: Unknown\n");
            break;
    }

    // Set scheduling policy to SCHED_FIFO with priority 10
    struct sched_param param;
    param.sched_priority = 10;
    sched_setscheduler(pid, SCHED_FIFO, &param);

    // Get and print new scheduling policy
    policy = sched_getscheduler(pid);
    printf("New policy: %d\n", policy);
    switch (policy) {
        case SCHED_FIFO:
            printf("New policy: SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("New policy: SCHED_RR\n");
            break;
        case SCHED_OTHER:
            printf("New policy: SCHED_OTHER\n");
            break;
        default:
            printf("New policy: Unknown\n");
            break;
    }

    return 0;
}

/*
--------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/29$ ./of29
Current policy: 0
Current policy: SCHED_OTHER
New policy: 1
New policy: SCHED_FIFO
--------------------------------------------------------------
*/



