/*
============================================================================
Name : 16.c
Author : Aditya Vijay Shinde
Description : Write a program to display the environmental variable of the user (use environ).
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>

extern char **environ;  // Declare the extern environ variable

int main() {
    char **env = environ;  // Get the environment variables

    // Loop through and print each environment variable
    while (*env) {
        printf("%s\n", *env);
        env++;
    }

    return 0;
}
