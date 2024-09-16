/*
============================================================================
Name : .c
Author : Aditya Vijay Shinde
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));      //eax a   edx d
    return ((uint64_t)hi << 32) | lo;
}

int main() {
    uint64_t start, end;

    // Get the starting time
    start = rdtsc();

    // Call getpid()
    pid_t pid = getpid();

    // Get the ending time
    end = rdtsc();

    // Calculate the time taken
    uint64_t time_taken = end - start;

    // Print the results
    printf("PID: %d\n", pid);
    printf("Time taken to execute getpid() (in TSC cycles): %lu\n", time_taken);

    return 0;
}

/*
---------------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/19$ ./of19
PID: 38776
Time taken to execute getpid() (in TSC cycles): 9563
--------------------------------------------------------------------------
*/
