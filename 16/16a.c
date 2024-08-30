/*
============================================================================
Name : 16.c
Author : Aditya Vijay Shinde
Description : Write a program to perform mandatory locking.
a. Implement write lock
b. Implement read lock
Date: 30th Aug, 2024.
============================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void apply_lock(int fd, short lock_type) {
    struct flock lock;
    lock.l_type = lock_type;   // F_WRLCK for write lock
    lock.l_whence = SEEK_SET;  // From the start of the file
    lock.l_start = 0;          // Specifies the offset (in bytes) from l_whence where the lock should start.
    lock.l_len = 0;            // Lock the entire file

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error applying lock");
    } 
    else if( lock_type == F_WRLCK ) {
        printf("Lock applied.\n");
    }
    else{
        printf("Lock released.\n");
    }
}

int main() {
    int fd = open("filelocking.txt", O_RDWR);  // Open the file in read-write mode
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Apply write lock 
    apply_lock(fd, F_WRLCK);  
    printf("File is locked. Press Enter to release the lock.\n");
    getchar();  // Wait for user input

    // Unlock the file before closing
    apply_lock(fd, F_UNLCK);
    close(fd);
    return 0;
}

