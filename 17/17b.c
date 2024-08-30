/*
============================================================================
Name : 17b.c
Author : Aditya Vijay Shinde
Description :Write a separate program, to open the file, implement write lock, 
read the ticket number, increment the number and print the new ticket number then close the file.
Date: 30th Aug, 2024.
============================================================================
*/
//Update Ticket program


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void apply_write_lock(int fd) {
    struct flock lock;
    lock.l_type = F_WRLCK;     // Write lock
    lock.l_whence = SEEK_SET;  // Lock from the start of the file
    lock.l_start = 0;          // Lock entire file
    lock.l_len = 0;            // Lock until the end of the file

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error applying write lock");
        return;
    }
    printf("Write lock applied.\n");
}

void release_lock(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;     // Unlock
    lock.l_whence = SEEK_SET;  // From the start of the file
    lock.l_start = 0;          // Unlock entire file
    lock.l_len = 0;            // Until the end

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error unlocking file");
        return;
    }
    printf("Lock released.\n");
}

int main() {
    int fd = open("ticket.txt", O_RDWR);  // Open file in read-write mode
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    apply_write_lock(fd);  // Apply write lock

    // Read current ticket number from file
    int ticket_number;
    lseek(fd, 0, SEEK_SET);  // Move file pointer to the beginning
    fscanf(fdopen(fd, "r"), "%d", &ticket_number);
    printf("Current ticket number: %d\n", ticket_number);

    // Increment ticket number
    ticket_number++;
    lseek(fd, 0, SEEK_SET);  // Move pointer back to the start for writing
    ftruncate(fd, 0);        // Clear the file before writing
    dprintf(fd, "%d", ticket_number);  // Write the new ticket number
    printf("New ticket number: %d\n", ticket_number);

    release_lock(fd);  // Release the write lock
    close(fd);         // Close the file

    return 0;
}

