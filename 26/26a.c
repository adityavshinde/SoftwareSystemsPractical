/*
============================================================================
Name : 26a.c
Author : Aditya Vijay Shinde
Description : Write a program to execute an executable program.
a. use some executable program
Date: 29th Aug, 2024.
============================================================================
*/


#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <name>\n", argv[0]);
        return 1;
    }
    printf("Hello, %s!\n", argv[1]);
    return 0;
}

