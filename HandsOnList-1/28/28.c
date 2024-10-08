/*
============================================================================
Name : 28.c
Author : Aditya Vijay Shinde
Description : Write a program to get maximum and minimum real time priority.
Date: 31st Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int main() {
    // Define the real-time scheduling policy
    int policy = SCHED_FIFO;  // Real-time scheduling policy: SCHED_FIFO or SCHED_RR

    // Get the maximum real-time priority
    int max_priority = sched_get_priority_max(policy);
    if (max_priority == -1) {
        perror("sched_get_priority_max");
        return 1;                  //failure
    }

    // Get the minimum real-time priority
    int min_priority = sched_get_priority_min(policy);
    if (min_priority == -1) {
        perror("sched_get_priority_min");
        return 1;                 //failure
    }

    // Print the results
    printf("Maximum real-time priority for policy %d: %d\n", policy, max_priority);
    printf("Minimum real-time priority for policy %d: %d\n", policy, min_priority);

    return 0;       //success
}

/*
----------------------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/28$ ./of28
Maximum real-time priority for policy 1: 99
Minimum real-time priority for policy 1: 1
----------------------------------------------------------------------------------
*/
