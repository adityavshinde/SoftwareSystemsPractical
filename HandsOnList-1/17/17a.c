/*
============================================================================
Name : 17a.c
Author : Aditya Vijay Shinde
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit.
Date: 30th Aug, 2024.
============================================================================
*/

//Store the ticket 

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("ticket.txt", O_WRONLY | O_CREAT, 0666);  // Open the file in write mode, create if not exists
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    int ticket_number = 1000;  // Starting ticket number
    dprintf(fd, "%d", ticket_number);  // Write ticket number to file
    printf("Ticket number %d stored in file.\n", ticket_number);

    close(fd);  // Close the file
    return 0;
}

/*
---------------------------------------------------------------------
output:
g3n1u5@g3n1u5:~/SS/SoftwareSystemsPractical/17$ ./of17a
Ticket number 1000 stored in file.
---------------------------------------------------------------------
*/
