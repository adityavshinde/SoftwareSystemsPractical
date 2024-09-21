/*
============================================================================
Name : 5.c
Author : Aditya Shinde
Description : Write a program to print the system limitation of
a. maximum length of the arguments to the exec family of functions.
b. maximum number of simultaneous process per user id.
c. number of clock ticks (jiffy) per second.
d. maximum number of open files
e. size of a page
f. total number of pages in the physical memory
g. number of currently available pages in the physical memory.
Date: 17th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

void print_sysconf(const char *description, int name) {
    long val = sysconf(name);
    if (val != -1) {
        printf("%s: %ld\n", description, val);
    } else {
        perror("sysconf failed");
    }
}

int main() {
    print_sysconf("Maximum length of arguments to exec", _SC_ARG_MAX);
    print_sysconf("Maximum number of processes per user", _SC_CHILD_MAX);
    print_sysconf("Number of clock ticks per second", _SC_CLK_TCK);
    print_sysconf("Maximum number of open files", _SC_OPEN_MAX);
    print_sysconf("Size of a memory page (bytes)", _SC_PAGESIZE);
    print_sysconf("Total number of pages in physical memory", _SC_PHYS_PAGES);
    print_sysconf("Available pages in physical memory", _SC_AVPHYS_PAGES);

    return 0;
}

/*
-------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./5_out
Maximum length of arguments to exec: 2097152
Maximum number of processes per user: 62386
Number of clock ticks per second: 100
Maximum number of open files: 1024
Size of a memory page (bytes): 4096
Total number of pages in physical memory: 4011488
Available pages in physical memory: 2685058
-------------------------------------------------------------------------
*/
