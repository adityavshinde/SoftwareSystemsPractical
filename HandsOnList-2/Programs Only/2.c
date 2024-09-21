/*
============================================================================
Name : 2.c
Author : Aditya Shinde
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date: 17th Sep, 2024.
============================================================================
*/
#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>

void print_limit(int resource, const char* name) {
    struct rlimit limit;
    
    if (getrlimit(resource, &limit) == 0) {
        printf("%s:\n", name);
        printf("  Soft limit: %ld\n", (long)limit.rlim_cur);
        printf("  Hard limit: %ld\n\n", (long)limit.rlim_max);
    } else {
        perror("getrlimit");
        exit(EXIT_FAILURE);
    }
}

int main() {

    print_limit(RLIMIT_CORE, "Maximum core file size (bytes)");
    print_limit(RLIMIT_FSIZE, "Maximum file size (bytes)");
    print_limit(RLIMIT_MEMLOCK, "Maximum locked memory (bytes)");
    print_limit(RLIMIT_NOFILE, "Maximum number of open files");
    print_limit(RLIMIT_STACK, "Maximum stack size (bytes)");
    print_limit(RLIMIT_NPROC, "Maximum number of processes");

    return 0;
}

/*
-------------------------------------------------------------------
OUTPUT:

g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./2_out
Maximum core file size (bytes):
  Soft limit: 0
  Hard limit: -1

Maximum file size (bytes):
  Soft limit: -1
  Hard limit: -1

Maximum locked memory (bytes):
  Soft limit: 2053881856
  Hard limit: 2053881856

Maximum number of open files:
  Soft limit: 1024
  Hard limit: 1048576

Maximum stack size (bytes):
  Soft limit: 8388608
  Hard limit: -1

Maximum number of processes:
  Soft limit: 62386
  Hard limit: 62386

-------------------------------------------------------------------
*/
