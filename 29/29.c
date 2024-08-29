/*
============================================================================
Name : 29.c
Author : Aditya Vijay Shinde
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,SCHED_RR).
Date: 29th Aug, 2024.
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

    // Set scheduling policy to SCHED_FIFO with priority 10
    struct sched_param param;
    param.sched_priority = 10;
    sched_setscheduler(pid, SCHED_FIFO, &param);

    // Get and print new scheduling policy
    policy = sched_getscheduler(pid);
    printf("New policy: %d\n", policy);

    return 0;
}

