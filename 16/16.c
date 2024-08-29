/*
============================================================================
Name : 16.c
Author : Aditya Vijay Shinde
Description : Write a program to perform mandatory locking.
a. Implement write lock
b. Implement read lock
Date: 29th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void apply_lock(int fd, short lock_type) {
    struct flock lock;
    lock.l_type = lock_type;   // F_WRLCK for write lock, F_RDLCK for read lock
    lock.l_whence = SEEK_SET;  // From the start of the file
    lock.l_start = 0;          // Start at the beginning
    lock.l_len = 0;            // Lock the entire file

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error applying lock");
    } else {
        printf("Lock applied.\n");
    }
}

int main() {
    int fd = open("testfile.txt", O_RDWR);  // Open the file in read-write mode
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Apply write lock (use F_RDLCK for read lock)
    apply_lock(fd, F_WRLCK);  
    printf("File is locked. Press Enter to release the lock.\n");
    getchar();  // Wait for user input

    // Unlock the file before closing
    apply_lock(fd, F_UNLCK);
    close(fd);
    return 0;
}

