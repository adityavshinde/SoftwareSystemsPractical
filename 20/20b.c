/*
============================================================================
Name : 20b.c
Author : Aditya Vijay Shinde
Description : Modify the priority with nice command.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid();  // Get the PID of the current process

    // Get and print the current priority
    int current_priority = getpriority(PRIO_PROCESS, pid);
    printf("Current priority: %d\n", current_priority);

    // Modify the priority (increase the priority value to decrease the priority of the process)
    int new_priority = current_priority + 10;  // Increase priority by 10
    if (setpriority(PRIO_PROCESS, pid, new_priority) == -1) {
        perror("setpriority");
        return 1;
    }

    // Print the new priority
    int updated_priority = getpriority(PRIO_PROCESS, pid);
    printf("Updated priority: %d\n", updated_priority);

    return 0;
}

