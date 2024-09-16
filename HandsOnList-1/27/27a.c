/*
============================================================================
Name : 27a.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
a. execl
Date: 30th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execl:\n");

    // Execute ls -Rl
    execl("/bin/ls", "ls", "-Rl", (char *)NULL);

    // If execl returns, an error occurred
    perror("execl");
    return 1;
}

/*
output:
Using execl:
.:
total 100
-rw-rw-r-- 1 g3n1u5 g3n1u5   549 Aug 31 16:13 27a.c
-rw-rw-r-- 1 g3n1u5 g3n1u5   548 Aug 29 18:45 27b.c
-rw-rw-r-- 1 g3n1u5 g3n1u5   586 Aug 29 18:45 27c.c
-rw-rw-r-- 1 g3n1u5 g3n1u5   596 Aug 29 18:45 27d.c
-rw-rw-r-- 1 g3n1u5 g3n1u5   596 Aug 29 18:45 27e.c
-rwxrwxr-x 1 g3n1u5 g3n1u5 16040 Aug 31 16:18 a.out
-rwxrwxr-x 1 g3n1u5 g3n1u5 16048 Aug 31 16:18 b.out
-rwxrwxr-x 1 g3n1u5 g3n1u5 16048 Aug 31 16:19 c.out
-rwxrwxr-x 1 g3n1u5 g3n1u5 16096 Aug 31 16:19 d.out
-rwxrwxr-x 1 g3n1u5 g3n1u5 16096 Aug 31 16:19 e.out
*/
