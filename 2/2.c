/*
============================================================================
Name : 2.c
Author : Aditya Vijay Shinde
Description : Write a simple program to execute in an infinite loop at the background. Go to /proc directory and
identify all the process related information in the corresponding proc directory.
Date: 28th Aug, 2024.
============================================================================
*/


#include <unistd.h>

int main() {
    if (fork() == 0) { // Child process
        while (1) {
            sleep(5); // Infinite loop with 5 seconds sleep
        }
    }
    return 0; // Parent process exits
}
