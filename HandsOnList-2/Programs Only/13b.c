/*
============================================================================
Name : 13b.c
Author : Aditya Shinde
Description : Write two programs: first program is waiting to catch SIGSTOP signal, the second program
will send the signal (using kill system call). Find out whether the first program is able to catch
the signal or not.
Date: 18th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    pid_t target_pid;
    printf("Enter the PID of the process to send SIGSTOP: ");
    scanf("%d", &target_pid);

    kill(target_pid, SIGSTOP);
    printf("Sent SIGSTOP to PID: %d\n", target_pid);

    sleep(2);  
    
    kill(target_pid, SIGCONT);
    printf("Sent SIGCONT to PID: %d\n", target_pid);

    return 0;
}

/*
-----------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./13b_out
Enter the PID of the process to send SIGSTOP: 94262
Sent SIGSTOP to PID: 94262
Sent SIGCONT to PID: 94262
-----------------------------------------------------------------------
*/
