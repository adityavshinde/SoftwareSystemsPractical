#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>

int main() {
    struct rlimit limit;

    if (getrlimit(RLIMIT_FSIZE, &limit) == 0) {
        printf("Old File Size Limits:\n");
        printf("  Soft limit: %ld bytes\n", (long)limit.rlim_cur);
        printf("  Hard limit: %ld bytes\n", (long)limit.rlim_max);
    } else {
        perror("getrlimit failed");
        exit(EXIT_FAILURE);
    }

    limit.rlim_cur = 1024 * 1024;  // 1 MB
    limit.rlim_max = 1024 * 1024; 

    if (setrlimit(RLIMIT_FSIZE, &limit) != 0) {
        perror("setrlimit failed");
        exit(EXIT_FAILURE);
    }

    printf("Successfully set the maximum file size limit to 1 MB.\n");

    if (getrlimit(RLIMIT_FSIZE, &limit) == 0) {
        printf("New File Size Limits:\n");
        printf("  Soft limit: %ld bytes\n", (long)limit.rlim_cur);
        printf("  Hard limit: %ld bytes\n", (long)limit.rlim_max);
    } else {
        perror("getrlimit failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

/*
------------------------------------------------------------------------
OUTPUT:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/HandsOnList-2$ ./3_out
Old File Size Limits:
  Soft limit: -1 bytes
  Hard limit: -1 bytes
Successfully set the maximum file size limit to 1 MB.
New File Size Limits:
  Soft limit: 1048576 bytes
  Hard limit: 1048576 bytes
------------------------------------------------------------------------
*/
