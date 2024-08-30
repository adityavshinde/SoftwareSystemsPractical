/*
============================================================================
Name : 13.c
Author : Aditya Vijay Shinde
Description : Write a program to wait for a STDIN for 10 seconds using select. 
Write a proper print statement to verify whether the data is available within 
10 seconds or not (check in $man 2 select).
Date: 29th Aug, 2024.
============================================================================
*/

#include <stdio.h>
#include <unistd.h>  // For STDIN_FILENO
#include <sys/select.h>  // For select()

int main() {
    fd_set read_fds;  // Set of file descriptors to monitor
    struct timeval t;  // Timeout for select

    // Initialize the file descriptor set
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);  // Monitor STDIN (fd 0)

    // Set the timeout to 10 seconds
    t.tv_sec = 10;   //seconds
    t.tv_usec = 0;   //microseconds

    // Wait for input on STDIN
    int val = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &t);    //select(Range of FD, Readability, Writability, Errors, Timeout)

    if (val == -1) {
        perror("select() error");
    } else if (val == 0) {
        printf("No data entered within 10 seconds, timed out.\n");
    } else {
        printf("Data is available, you entered something!\n");
    }

    return 0;
}

/*
--------------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/13$ ./of13
touch file.txt
Data is available, you entered something!
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/13$ touch file.txt
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/13$ ls
13.c  file.txt  of13
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/13$ ./of13
No data entered within 10 seconds, timed out.
--------------------------------------------------------------------------
*/

