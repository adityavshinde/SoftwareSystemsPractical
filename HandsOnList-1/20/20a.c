/*
============================================================================
Name : 20a.c
Author : Aditya Vijay Shinde
Description : Find out the priority of your running program.
Date: 30th Aug, 2024.
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

/*
----------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/20$ ./of20a
Current priority of PID 39913: 0
---------------------------------------------------------------------
*/
