/*
============================================================================
Name : 30.c
Author : Aditya Vijay Shinde
Description : Write a program to run a script at a specific time using a Daemon process.
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void create_daemon() {
    if (fork() > 0) exit(0);  // Parent exits
    setsid();  // Start new session
    chdir("/");  // Change to root directory
}

int main() {
    int hour = 12, minute = 0;  // Set target time
    const char *script = "/path/to/script.sh";  // Script path

    create_daemon();

    while (1) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        
        if (t->tm_hour == hour && t->tm_min == minute) {
            system(script);
            sleep(60);  // Avoid multiple executions in the same minute
        }
        sleep(1);  // Check time every second
    }

    return 0;
}

