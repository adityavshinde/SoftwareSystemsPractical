/*
============================================================================
Name : 30.c
Author : Aditya Vijay Shinde
Description : Write a program to run a script at a specific time using a Daemon process.
Date: 31st Aug, 2024.
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
    int hour = 15, minute = 21;  // Set target time
    const char *script = "/home/g3n1u5/SS/SoftwareSystemsPractical/30/script.sh";  // Script path

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

/*
---------------------------------------------------------
output:
g3n1u5@g3n1u5:/var/log$ cat daemon.log
Daemon executed at Sat Aug 31 03:21:00 PM IST 2024
---------------------------------------------------------
*/
