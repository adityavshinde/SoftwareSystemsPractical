/*
============================================================================
Name : 27b.c
Author : Aditya Vijay Shinde
Description : Write a program to execute ls -Rl by the following system calls
b. execlp
Date: 30th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Using execlp:\n");

    // Execute ls -Rl
    execlp("ls", "ls", "-Rl", (char *)NULL);

    // If execlp returns, an error occurred
    perror("execlp");
    return 1;
}
/*
output:
Using execlp:
.:
total 100
-rw-rw-r-- 1 g3n1u5 g3n1u5  1109 Aug 31 16:20 27a.c
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
